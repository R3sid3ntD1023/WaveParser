#include "WaveParser.h"
#include "Utils.h"
#include "id3_FrameHandler.h"


WaveParser::WaveParser(const char* filename)
{
    f = fopen(filename, "rb");
}

WaveParser::~WaveParser()
{
    fclose(f);
}

wave_t WaveParser::parse()
{
   /* if (std::endian::native == std::endian::little)
    {
        printf("Little endian\n");
    }*/

    wave_t wave;
    parse_header(&wave.header);
   // parse_fmt(&wave.fmt);


    char text[5] = { 0 };

    chunk_info_t chunk_info;
    unsigned chunk_val = 0;

    while (fread(&chunk_info, sizeof(chunk_info_t), 1, f) != 0)
    {
        chunk_val = utils::char_array_to_unsigned(chunk_info.id);
        //printf("<id : %.*s, size %d>\n", 4, chunk_info.id, chunk_info.size);
        switch (chunk_val)
        {
            case DATA_MARKER:
            {
                //printf("getting sound data...\n");

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
                //printf("list type = %.*s\n", 4, list.type);
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

    //printf("%.*s \n", 4, header->id);
    //printf("%d \n", header->size);
   
    auto id = utils::char_array_to_unsigned(header->id);
    auto format = utils::char_array_to_unsigned(header->format);

    if (id != RIFF_TAG)
    {
        printf("File is not RIFF\n");
        return;
    }


    if (format != WAVE_TAG)
    {
        printf("File is not WAVE\n");
        return;
    }

}

void WaveParser::parse_fmt(fmt_chunk_t* fmt_chunck)
{
    fread(fmt_chunck, sizeof(fmt_chunk_t), 1, f);

   /* printf("format : %d \n", fmt_chunck->audio_format);
    printf("channels : %d \n", fmt_chunck->num_channels);
    printf("sample rate : %d \n", fmt_chunck->sample_rate);
    printf("byte rate : %d \n", fmt_chunck->byte_rate);
    printf("block align : %d \n", fmt_chunck->block_align);
    printf("bits per sample : %d \n", fmt_chunck->bits_per_sample);*/

}

void WaveParser::parse_list(list_chunk_t* list_chunk)
{
    int bytesize = 0;

    
    chunk_info_t chunk_info;
    while (fread(&chunk_info.id, sizeof(char), 4, f) != 0)
    {
        unsigned chunk_val = utils::char_array_to_unsigned(chunk_info.id);
        if (chunk_val == ID3_MARKER)
        {
            //printf("found id3 tag\n");

            id3_t& id3 = list_chunk->id3_chunk;
            fread(&chunk_info.size, sizeof(int), 1, f);
            id3.header = chunk_info;
            parse_id3(&id3);
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

           // printf("%.*s : size = %d, data = %.*s\n", 4, chunk_info.id, chunk_info.size, chunk_info.size, extra_chunk->data);
            subchunk = extra_chunk;
        }
    }
}

void WaveParser::parse_id3(id3_t* id3)
{
    fread(&id3->id3, sizeof(id3_header_t), 1, f);

    //printf("id3 type : %.*s\n", 3, id3->id3.identifier);
    //printf("id3 version : %d.%d\n", id3->id3.version[0], id3->id3.version[1]);
    //printf("id3 flags : %d, %d, %d\n", id3->id3.flags[0], id3->id3.flags[1], id3->id3.flags[2]);
   
    id3_tag_t tag;
    while (fread(&tag, sizeof(id3_tag_t), 1, f) != 0)
    {
        unsigned id_val = utils::byte_array_to_unsigned(tag.id);
        auto frame = id3_FrameHandler::process(f, id_val);

        if (frame)
        {
            id3->tags[frame->get_name()] = frame;
        }
        else
        {
            printf("no impl for %.*s found!", 4, tag.id);
        }
    }
}
