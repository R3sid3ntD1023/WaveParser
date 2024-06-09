#include "WaveParser.h"
#include "Utils.h"
#include "id3_FrameHandler.h"


WaveParser::WaveParser(const char* filename)
{
    f = fopen(filename, "rb");
    LOG(info, "parsing wave file {}", filename);
}

WaveParser::~WaveParser()
{
    fclose(f);
}

wave_t WaveParser::parse()
{

    wave_t wave;
    parse_header(&wave.header);

    char text[5] = { 0 };

    chunk_info_t chunk_info;
    unsigned chunk_val = 0;

    while (fread(&chunk_info, sizeof(chunk_info_t), 1, f) != 0)
    {
        chunk_val = utils::char_array_to_unsigned(chunk_info.id);
       
        switch (chunk_val)
        {
            case DATA_MARKER:
            {
                LOG(info, "getting sound data...");

                chunk_data_t data_chunk;
                data_chunk.header = chunk_info;
                data_chunk.data = new byte[chunk_info.size];
                fread(data_chunk.data, sizeof(byte), chunk_info.size, f);
                wave.data = data_chunk;
                break;
            }
            case LIST_MARKER:
            {
                list_chunk_t list;
                fread(&list.type, sizeof(char), 4, f);
                parse_list(&list);

                wave.list = list;
                break;
            }
            case FMT_MARKER:
            {
               
                parse_fmt(&wave.fmt);
                break;
            }
            default:
                break;
        }

        
    };

    return wave;
}

void WaveParser::parse_header(wave_header_t* header)
{
    fread(header, sizeof(wave_header_t), 1, f);

    auto id = utils::char_array_to_unsigned(header->id);
    auto format = utils::char_array_to_unsigned(header->format);

    if (id != RIFF_TAG)
    {
        LOG(error, "File is not RIFF");
        return;
    }


    if (format != WAVE_TAG)
    {
        LOG(error, "File is not WAVE");
        return;
    }

}

void WaveParser::parse_fmt(fmt_chunk_t* fmt_chunck)
{
    fread(fmt_chunck, sizeof(fmt_chunk_t), 1, f);

    LOG(info, "format : {}",             fmt_chunck->audio_format);
    LOG(info, "channels : {}",           fmt_chunck->num_channels);
    LOG(info, "sample rate : {}",        fmt_chunck->sample_rate);
    LOG(info, "byte rate : {}",          fmt_chunck->byte_rate);
    LOG(info, "block align : {}",        fmt_chunck->block_align);
    LOG(info, "bits per sample : {}",    fmt_chunck->bits_per_sample);

}

void WaveParser::parse_list(list_chunk_t* list_chunk)
{
    long id3_tag_pos = 0;

    
    chunk_info_t chunk_info;
    while (fread(&chunk_info.id, sizeof(char), 4, f) != 0)
    {
        unsigned chunk_val = utils::char_array_to_unsigned(chunk_info.id);
        if (chunk_val == ID3_MARKER)
        {
           
            int id3_size = 0;
            fread(&id3_size, sizeof(int), 1, f);
            id3_tag_pos = ftell(f);
            fseek(f, id3_size, SEEK_CUR);
        }
        else
        {
            fread(&chunk_info.size, sizeof(int), 1, f);
            chunk_ptr& subchunk = list_chunk->sub_chunks[chunk_val];
            auto extra_chunk = make_chunk(chunk_data_t);
            extra_chunk->header = chunk_info;
           
            if (extra_chunk->header.size)
            {
                extra_chunk->data = new byte[chunk_info.size];
                fread(extra_chunk->data, sizeof(byte), chunk_info.size, f);
            }

            subchunk = extra_chunk;
        }
    }

    if (id3_tag_pos > 0)
    {
        LOG(info, "parsing id3 tags");

        fseek(f, id3_tag_pos, SEEK_SET);

        id3_t& id3 = list_chunk->id3_chunk;
        fread(&id3.header, sizeof(id3_header_t), 1, f);
       
        LOG(info, "id3 version : {:d}", id3.header.version);

        parse_id3(&id3);
    }
}

void WaveParser::parse_id3(id3_t* id3)
{
    id3_tag_t tag;
    while (fread(&tag, sizeof(id3_tag_t), 1, f) != 0)
    {
        unsigned id_val = utils::byte_array_to_unsigned(tag.id);
        auto frame = id3_FrameHandler::process(f, id_val);

        if (frame)
        {
            id3->tags[frame->get_name()] = frame;
            frame->print();
        }
        else
        {
            LOG(warn ,"no impl for {0:4} found!\n", tag.id);
        }
    }
}
