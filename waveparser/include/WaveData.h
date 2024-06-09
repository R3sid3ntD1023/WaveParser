#pragma once

#include "Core.h"
#include "id3_Frames.h"
#include "id3_Tag.h"

struct chunk_info_t
{
	char id[4] = { 0 };
	int size;
};

struct chunk_t
{
	virtual ~chunk_t() = default;

	chunk_info_t header;
	
	const char* get_name() const { return header.id; }
};

struct chunk_data_t : public chunk_t
{
	byte* data = nullptr;
};

typedef std::shared_ptr<chunk_t> chunk_ptr;
#define make_chunk(cls) std::make_shared<cls>()

struct fmt_chunk_t
{
	short audio_format;
	short num_channels;
	int sample_rate;
	int byte_rate;
	short block_align;
	short bits_per_sample;
};

struct wave_header_t
{
	char id[4] = { 0 };
	int size;
	char format[4] = { 0 };
};


struct id3_header_t
{
	char identifier[3];
	byte version[2];
	id3_flag flags[3];
	byte ignored[2];
};

struct id3_t : public chunk_t
{
	id3_header_t id3;
	std::unordered_map<std::string, id3_frame_ptr> tags;
};

struct list_chunk_t
{
	char type[4] = { 0 };
	std::unordered_map<unsigned, chunk_ptr> sub_chunks;
	id3_t id3_chunk;
};



struct wave_t
{
	wave_header_t header;
	fmt_chunk_t fmt;
	list_chunk_t list;
	chunk_data_t data;

	int get_num_samples_per_channel() const 
	{
		short bits_per_sample = fmt.bits_per_sample / 8;
		return data.header.size / bits_per_sample / fmt.num_channels; 
	}

	short* get_samples() const { return (short*)data.data; }

	float get_length() const { return (float)get_num_samples_per_channel() / (float)fmt.sample_rate; }

	int get_buffer_size() const { return 2 * fmt.num_channels * get_num_samples_per_channel(); }
};

