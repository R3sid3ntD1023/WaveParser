#pragma once

#include "Core.h"
#include "id3_Frame.h"
#include "id3_Tag.h"

namespace WAVE
{

	struct chunk_info_t
	{
		byte_t id[4] = {0};
		unsigned size = 0;
	};

	struct chunk_t
	{
		chunk_t() = default;
		chunk_t(const chunk_info_t &info)
			: header(info)
		{
		}

		~chunk_t()
		{
			delete[] data;
		}

		chunk_info_t header;
		byte_t *data = nullptr;

		std::string get_name() const { return std::string(header.id, 4); }
		std::string get_data() const { return std::string(data, header.size); }
	};

	struct fmt_chunk_t
	{
		short audio_format;
		short num_channels;
		unsigned sample_rate;
		unsigned byte_rate;
		short block_align;
		short bits_per_sample;
	};

	struct wave_header_t
	{
		byte_t id[4] = {0};
		unsigned size;
		byte_t format[4] = {0};
	};

	struct id3_header_t
	{
		byte_t identifier[3]; /*ID3v2/file header 	ID3*/
		byte_t version[2];	  /*ID3v2 version 		$03 00*/
		id3_flag flags;		  /*ID3v2 flags 		%abc00000*/
		byte_t size[4];		  /*ID3v2 size			4 * %0xxxxxxxx*/
	};

	struct id3_extended_header_t
	{
		byte_t size[4];
		byte_t flags[2];
		byte_t padding[4];
	};

	struct id3_t
	{
		using Tags = std::unordered_map<std::string, id3_frame_ptr>;

		id3_header_t header;

		bool has_tag(const std::string &name) const { return tags.contains(name); }
		id3_frame_ptr get_tag(const std::string &name) const { return has_tag(name) ? tags.at(name) : nullptr; }

		template <typename T>
		std::shared_ptr<T> get_tag(const std::string &name) const { return std::dynamic_pointer_cast<T>(get_tag(name)); }

		const Tags &get_tags() const { return tags; }

	private:
		Tags tags;

		friend class Parser;
	};

	struct list_chunk_t
	{
		byte_t type[4] = {0};
		std::vector<std::shared_ptr<chunk_t>> sub_chunks;
		id3_t id3_chunk;
	};

	struct wave_t
	{
		wave_header_t header;
		fmt_chunk_t fmt;
		list_chunk_t list;
		chunk_t data;
		std::vector<std::shared_ptr<chunk_t>> extrachunks;

		unsigned get_num_samples_per_channel() const
		{
			short bits_per_sample = fmt.bits_per_sample / 8;
			return data.header.size / bits_per_sample / fmt.num_channels;
		}

		unsigned get_num_samples() const
		{
			short bits_per_sample = fmt.bits_per_sample / 8;
			return data.header.size / bits_per_sample;
		}

		short *get_samples() const { return reinterpret_cast<short *>(data.data); }

		float get_length() const { return (float)get_num_samples_per_channel() / (float)fmt.sample_rate; }

		unsigned get_buffer_size() const { return 2 * fmt.num_channels * get_num_samples_per_channel(); }
	};
}
