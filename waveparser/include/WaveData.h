#pragma once

#include "Core.h"
#include "id3_Frame.h"

namespace waveparser
{
	enum ID3Flags : byte_t
	{
		None = 0x00,
		FooterPresent = 0x10,
		ExperimentalIndictor = 0x20,
		ExtentedHeader = 0x40,
		Unsynchronisation = 0x80
	};

	inline std::string ToString(ID3Flags flags)
	{
		std::string result;
		if (flags & None)
			result += "None ";
		if (flags & FooterPresent)
			result += "FooterPresent ";
		if (flags & ExperimentalIndictor)
			result += "ExperimentalIndictor ";
		if (flags & ExtentedHeader)
			result += "ExtentedHeader ";
		if (flags & Unsynchronisation)
			result += "Unsynchronisation ";
		return result;
	}

#pragma pack(push, 1)

	struct RIFFHeader
	{
		byte_t ChunkId[4] = {0, 0, 0, 0};
		uint32_t ChunkSize;
		byte_t Format[4] = {0, 0, 0, 0};
	};

	struct FMTChunk
	{
		short AudioFormat = 0;
		short NumChannels = 0;
		unsigned SampleRate = 0;
		unsigned ByteRate = 0;
		short BlockAlign = 0;
		short BitsPerSample = 0;
	};

	struct ChunkHeader
	{
		byte_t SubChunkId[4] = {0, 0, 0, 0};

		uint32_t SubChunkSize = 0;
	};

	struct ID3Header
	{
		byte_t Identifier[3]{0, 0, 0}; /*ID3v2/file header 	ID3*/
		byte_t VersionMajor;		   /*ID3v2 version 	hex	$03 00*/
		byte_t VersionRevision;		   /*ID3v2 version 	hex	$03 00*/
		ID3Flags Flags;				   /*ID3v2 flags 		%abcd00000*/
		byte_t Size[4];				   /*ID3v2 size			4 * %0xxxxxxxx*/
	};

	struct ID3ExtendedHeader
	{
		byte_t Size[4]{0, 0, 0, 0};
		byte_t Flags[2]{0, 0};
		byte_t PaddingSize[4]{0, 0, 0, 0};
	};

#pragma pack(pop)

	struct Chunk
	{
		Chunk(const ChunkHeader &chunkHeader)
			: Header(chunkHeader)
		{
		}

		ChunkHeader Header;

		std::vector<byte_t> Data;

		std::string GetName() const { return std::string(reinterpret_cast<const char *>(Header.SubChunkId), 4); }
	};

	struct ID3
	{
		ID3Header Header;

		void AddTag(std::shared_ptr<ID3Frame> tag) { Tags.push_back(tag); }

		bool HasTags(const std::string &name) const { return !GetTagsByName(name).empty(); }

		std::vector<std::shared_ptr<ID3Frame>> GetTagsByName(const std::string &name) const
		{
			std::vector<std::shared_ptr<ID3Frame>> result;
			auto hash = std::hash<std::string>{}(name);

			for (auto &tag : Tags)
			{
				if (tag->GetName() == name)
					result.push_back(tag);
			}
			return result;
		}

		const auto &GetTags() const { return Tags; }

	private:
		std::vector<std::shared_ptr<ID3Frame>> Tags;

		friend class Parser;
	};

	struct LISTChunk
	{
		byte_t Type[4] = {0};

		std::vector<std::shared_ptr<Chunk>> SubChunks;
	};

	struct Wave
	{
		RIFFHeader Header;

		FMTChunk Fmt;

		LISTChunk List;

		ID3 Id3Chunk;

		std::shared_ptr<Chunk> Data;

		std::vector<std::shared_ptr<Chunk>> ExtraChunks;

		short GetAudioFormat() const { return Fmt.AudioFormat; }

		short GetNumChannels() const { return Fmt.NumChannels; }

		uint32_t GetSampleRate() const { return Fmt.SampleRate; }

		uint32_t GetBitRate() const { return Fmt.SampleRate * Fmt.BitsPerSample; }

		uint32_t GetNumSamplesPerChannel() const
		{
			if (!Data || Fmt.BitsPerSample == 0)
				return 0;

			const auto bytesPerSample = Fmt.BitsPerSample / 8;

			return bytesPerSample ? Data->Data.size() / bytesPerSample : 0;
		}

		uint32_t GetNumSamples() const
		{
			if (Fmt.NumChannels <= 0)
				return 0;

			return GetNumSamples() / Fmt.NumChannels;
		}

		const std::vector<byte_t> &GetData() const { return Data->Data; }

		float GetLength() const { return (float)GetNumSamplesPerChannel() / (float)GetSampleRate(); }
	};
} // namespace waveparser
