#pragma once

#include "Core.h"
#include "id3_Frame.h"

namespace WAVE
{
	struct ChunkInfo
	{
		byte_t id[4] = {'\0', '\0', '\0', '\0'};

		uint32_t size = 0;
	};

	struct Chunk
	{
		Chunk(const ChunkInfo &info)
			: header(info)
		{
		}

		~Chunk() { delete[] data; }

		ChunkInfo header;

		byte_t *data = nullptr;

		std::string get_name() const { return std::string((char *)header.id, 4); }

		byte_t *get_data() const { return data; }
	};

	struct FMT_Chunk
	{
		short audio_format = 0;
		short num_channels = 0;
		unsigned sample_rate = 0;
		unsigned byte_rate = 0;
		short block_align = 0;
		short bits_per_sample = 0;
	};

	struct WaveHeader
	{
		byte_t id[4] = {0};
		unsigned size = 0;
		byte_t format[4] = {0};
	};

	enum ID3Flags : byte_t
	{
		None = 0x00,
		FooterPresent = 0x10,
		ExperimentalIndictor = 0x20,
		ExtentedHeader = 0x40,
		Unsynchronisation = 0x80
	};

#pragma pack(push, 1)
	struct ID3_Header
	{
		byte_t identifier[3]{0, 0, 0}; /*ID3v2/file header 	ID3*/
		byte_t versionMajor;		   /*ID3v2 version 	hex	$03 00*/
		byte_t versionRevision;		   /*ID3v2 version 	hex	$03 00*/
		ID3Flags flags;				   /*ID3v2 flags 		%abcd00000*/
		byte_t size[4];				   /*ID3v2 size			4 * %0xxxxxxxx*/
	};
#pragma pack(pop)

	struct ID3_ExtendedHeader
	{
		byte_t size[4]{0, 0, 0, 0};
		byte_t flags[2]{0, 0};
		byte_t paddingSize[4]{0, 0, 0, 0};
	};

	struct ID3
	{
		ID3_Header header;

		bool has_tag(const std::string &name) const
		{
			auto hash = std::hash<std::string>{}(name);
			return tags.contains(hash);
		}

		ID3_Frame_T *get_tag(const std::string &name) const
		{
			auto hash = std::hash<std::string>{}(name);
			return has_tag(name) ? tags.at(hash).get() : nullptr;
		}

		template <typename T>
			requires(std::is_base_of_v<ID3_Frame_T, T>)
		T *get_tag(const std::string &name) const
		{
			return dynamic_cast<T>(get_tag(name));
		}

		const auto &get_tags() const { return tags; }

	private:
		std::unordered_map<uint64_t, std::shared_ptr<ID3_Frame_T>> tags;

		friend class Parser;
	};

	struct ListChunk
	{
		byte_t type[4] = {0};

		std::vector<std::shared_ptr<Chunk>> sub_chunks;

		ID3 id3_chunk;
	};

	struct wave_t
	{
		WaveHeader header;

		FMT_Chunk fmt;

		ListChunk list;

		std::shared_ptr<Chunk> data;

		std::vector<std::shared_ptr<Chunk>> extrachunks;

		uint32_t get_num_samples_per_channel() const
		{
			short bits_per_sample = fmt.bits_per_sample / 8;
			if (!data)
				return bits_per_sample / fmt.num_channels;

			return data->header.size / bits_per_sample / fmt.num_channels;
		}

		uint32_t get_num_samples() const
		{
			short bits_per_sample = fmt.bits_per_sample / 8;
			if (!data)
				return bits_per_sample;

			return data->header.size / bits_per_sample;
		}

		short *get_samples() const { return (short *)(data->data); }

		float get_length() const { return (float)get_num_samples_per_channel() / (float)fmt.sample_rate; }

		size_t get_buffer_size() const { return 2 * fmt.num_channels * get_num_samples_per_channel(); }
	};
} // namespace WAVE
