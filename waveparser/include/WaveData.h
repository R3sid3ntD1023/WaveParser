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
	chunk_t() = default;
	chunk_t(const chunk_info_t& info);
	chunk_t(const chunk_t& other);
	virtual ~chunk_t();

	chunk_info_t header;
	byte* data = nullptr;
	
	std::string get_name() const { return std::string(header.id, 4); }

	chunk_t& operator=(const chunk_t& rhs);
};

typedef std::shared_ptr<chunk_t> chunk_ptr;
#define make_chunk(... ) std::make_shared<chunk_t>(__VA_ARGS__)

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
	byte version;
	byte flags[6];
	
};

struct id3_t
{
	id3_header_t header;
	std::unordered_map<std::string, id3_frame_ptr> tags;

	bool has_tag(const std::string& name) const { return tags.contains(name); }
	id3_frame_ptr get_tag(const std::string& name) const { return has_tag(name) ? tags.at(name) : nullptr; }

	template<typename T>
	std::shared_ptr<T> get_tag(const std::string& name) const { return std::dynamic_pointer_cast<T>(get_tag(name)); }
};

struct list_chunk_t
{
	char type[4] = { 0 };
	std::unordered_map<unsigned, chunk_ptr> sub_chunks;
	id3_t id3_chunk;
};



struct wave_t
{
	wave_header_t header{};
	fmt_chunk_t fmt{};
	list_chunk_t list{};
	chunk_t data{};
	std::vector<chunk_ptr> extrachunks{};

	int get_num_samples_per_channel() const 
	{
		short bits_per_sample = fmt.bits_per_sample / 8;
		return data.header.size / bits_per_sample / fmt.num_channels; 
	}

	int get_num_samples() const
	{
		short bits_per_sample = fmt.bits_per_sample / 8;
		return data.header.size / bits_per_sample;
	}

	short* get_samples() const { return (short*)data.data; }

	float get_length() const { return (float)get_num_samples_per_channel() / (float)fmt.sample_rate; }

	int get_buffer_size() const { return 2 * fmt.num_channels * get_num_samples_per_channel(); }
};

