#include "WaveParser.h"
#include "Utils.h"
#include "id3_FrameHandler.h"

WaveParser::WaveParser(const char *filename)
{
    f = fopen(filename, "rb");
    WAVE_LOG(info, "parsing wave file {}", filename);
}

WaveParser::~WaveParser()
{
    fclose(f);
}

bool WaveParser::parse(wave_t *wave)
{
    if (!wave || !parse_header(&wave->header))
    {
        return false;
    }

    chunk_info_t chunk_info;
    unsigned chunk_val = 0;

    while (fread(&chunk_info, sizeof(chunk_info_t), 1, f) != 0)
    {
        chunk_val = utils::char_array_to_unsigned(chunk_info.id);

        switch (chunk_val)
        {
        case DATA_MARKER:
        {
            WAVE_LOG(info, "getting sound data...");

            chunk_t chunk = chunk_t(chunk_info);
            parse_chunk(&chunk);
            wave->data = chunk;
            break;
        }
        case LIST_MARKER:
        {
            list_chunk_t list;
            fread(&list.type, sizeof(char), 4, f);
            parse_list(&list);

            wave->list = list;
            break;
        }
        case FMT_MARKER:
        {

            parse_fmt(&wave->fmt);
            break;
        }
        default:
        {
            chunk_ptr chunk = make_chunk(chunk_info);
            parse_chunk(chunk.get());
            wave->extrachunks.push_back(chunk);
            break;
        }
        }
    };

    return false;
}

bool WaveParser::parse_header(wave_header_t *header)
{
    fread(header, sizeof(wave_header_t), 1, f);

    auto id = utils::char_array_to_unsigned(header->id);
    auto format = utils::char_array_to_unsigned(header->format);

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

void WaveParser::parse_fmt(fmt_chunk_t *fmt_chunck)
{
    fread(fmt_chunck, sizeof(fmt_chunk_t), 1, f);

    WAVE_LOG(info, "format : {}", fmt_chunck->audio_format);
    WAVE_LOG(info, "channels : {}", fmt_chunck->num_channels);
    WAVE_LOG(info, "sample rate : {}", fmt_chunck->sample_rate);
    WAVE_LOG(info, "byte rate : {}", fmt_chunck->byte_rate);
    WAVE_LOG(info, "block align : {}", fmt_chunck->block_align);
    WAVE_LOG(info, "bits per sample : {}", fmt_chunck->bits_per_sample);
}

void WaveParser::parse_list(list_chunk_t *list_chunk)
{
    long id3_tag_pos = 0;

    chunk_info_t chunk_info;
    while (fread(&chunk_info, sizeof(chunk_info_t), 1, f) != 0)
    {
        unsigned chunk_val = utils::char_array_to_unsigned(chunk_info.id);
        switch (chunk_val)
        {
        case ID3_MARKER:
        {
            id3_tag_pos = ftell(f);
            fseek(f, chunk_info.size, SEEK_CUR);
            break;
        }
        default:
        {
            chunk_ptr &subchunk = list_chunk->sub_chunks[chunk_val];
            subchunk = make_chunk(chunk_info);
            parse_chunk(subchunk.get());
            break;
        }
        }
    }

    if (id3_tag_pos > 0)
    {
        WAVE_LOG(info, "parsing id3 tags");

        fseek(f, id3_tag_pos, SEEK_SET);

        id3_t &id3 = list_chunk->id3_chunk;
        fread(&id3.header, sizeof(id3_header_t), 1, f);

        WAVE_LOG(info, "id3 version : {:d}", id3.header.version);

        parse_id3(&id3);
    }
}

void WaveParser::parse_id3(id3_t *id3)
{
    id3_tag_t tag;
    while (fread(&tag, sizeof(id3_tag_t), 1, f) != 0)
    {
        unsigned id_val = utils::byte_array_to_unsigned(tag.id);
        auto frame = id3_registry::get().get_id3_tag(id_val);

        if (frame)
        {
            frame->process_data(f);
            id3->tags[frame->get_name()] = frame;
            frame->print();
        }
        else
        {
            WAVE_LOG(warn, "no impl for {0:4} found!\n", tag.id);
        }
    }
}

void WaveParser::parse_chunk(chunk_t *chunk)
{
    if (!chunk)
        return;

    if (chunk->header.size)
    {
        fread(chunk->data.Data, sizeof(byte_t), chunk->header.size, f);
    }
}
