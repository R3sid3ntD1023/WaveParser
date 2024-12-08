#include "WaveParser.h"
#include "utility/Utils.h"
#include "id3_FrameHandler.h"
#include "Version.h"

namespace WAVE
{
    Parser::Parser(const char *filename)
    {
        stream = std::ifstream(filename, std::ios::binary | std::ios::in);
        WAVE_LOG(info, "parsing wave file {}", filename);
    }

    bool Parser::parse(wave_t &wave)
    {
        if (!parse_header(wave.header))
        {
            return false;
        }

        chunk_info_t chunk_info;
        unsigned chunk_val = 0;

        while (stream.read((char *)&chunk_info, sizeof(chunk_info_t)))
        {
            chunk_val = utils::char_array_to_unsigned(chunk_info.id);

            switch (chunk_val)
            {
            case DATA_MARKER:
            {
                WAVE_LOG(info, "getting sound data...");

                chunk_t chunk = chunk_t(chunk_info);
                parse_chunk(chunk);
                wave.data = chunk;
                break;
            }
            case LIST_MARKER:
            {
                list_chunk_t list;
                stream.read(list.type, 4);
                parse_list(list);

                wave.list = list;
                break;
            }
            case FMT_MARKER:
            {

                parse_fmt(wave.fmt);
                break;
            }
            default:
            {
                auto chunk = std::make_shared<chunk_t>(chunk_info);
                parse_chunk(*chunk);
                wave.extrachunks.push_back(chunk);
                break;
            }
            }
        };

        return true;
    }

    bool Parser::parse_header(wave_header_t &header)
    {
        stream.read((char *)&header, sizeof(wave_header_t));

        auto id = utils::char_array_to_unsigned(header.id);
        auto format = utils::char_array_to_unsigned(header.format);

        if (id != RIFF_TAG)
        {
            WAVE_LOG(error, "File is not RIFF");
            return false;
        }

        if (format != WAVE_TAG)
        {
            WAVE_LOG(error, "File is not WAVE");
            return false;
        }

        return true;
    }

    void Parser::parse_fmt(fmt_chunk_t &fmt_chunck)
    {
        stream.read((char *)&fmt_chunck, sizeof(fmt_chunk_t));

        WAVE_LOG(info, "format : {}", fmt_chunck.audio_format);
        WAVE_LOG(info, "channels : {}", fmt_chunck.num_channels);
        WAVE_LOG(info, "sample rate : {}", fmt_chunck.sample_rate);
        WAVE_LOG(info, "byte rate : {}", fmt_chunck.byte_rate);
        WAVE_LOG(info, "block align : {}", fmt_chunck.block_align);
        WAVE_LOG(info, "bits per sample : {}", fmt_chunck.bits_per_sample);
    }

    void Parser::parse_list(list_chunk_t &list_chunk)
    {
        chunk_info_t chunk_info;
        while (stream.read((char *)&chunk_info, sizeof(chunk_info_t)))
        {
            unsigned chunk_val = utils::char_array_to_unsigned(chunk_info.id);
            WAVE_LOG(info, "Chunk {} : {}", std::string(chunk_info.id, 4), chunk_info.size);

            switch (chunk_val)
            {
            case ID3_MARKER:
            {
                id3_t &id3 = list_chunk.id3_chunk;
                stream.read((char *)&id3.header, sizeof(id3_header_t));

                Version version(2, id3.header.version[0], id3.header.version[1]);
                WAVE_LOG(info, "id3 version : {}", version.to_string());
                WAVE_LOG(info, "id3 flags : {}", (unsigned)id3.header.flags);

                parse_id3(id3, version);
                break;
            }
            default:
            {
                auto chunk = std::make_shared<chunk_t>(chunk_info);
                parse_chunk(*chunk);
                list_chunk.sub_chunks.push_back(chunk);
                break;
            }
            }
        }
    }

    void Parser::parse_id3(id3_t &id3, const Version &version)
    {
        id3_tag_t tag;
        while (stream.read((char *)&tag, sizeof(id3_tag_t)))
        {
            unsigned id_val = utils::byte_array_to_unsigned(tag.id);

            WAVE_LOG(info, "Frame {}", std::string(tag.id, 4));

            auto frame = id3_registry::get().get_id3_tag(version, id_val);

            if (frame)
            {
                frame->process_data(stream);
                id3.tags[frame->get_name()] = frame;
            }
            else
            {
                WAVE_LOG(warn, "no impl for {0:4} found!\n", tag.id);
            }
        }
    }

    void Parser::parse_chunk(chunk_t &chunk)
    {
        auto size = chunk.header.size;
        if (size)
        {
            chunk.data = new byte_t[size + 1];
            stream.read(chunk.data, size);
        }
    }
}