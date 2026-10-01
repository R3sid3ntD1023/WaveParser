#include "WaveParser.h"
#include "Version.h"
#include "id3_FrameHandler.h"
#include "utility/Utils.h"

namespace waveparser
{
	Parser::Parser(const std::filesystem::path &filePath)
	{
		Stream = std::ifstream(filePath, std::ios::binary | std::ios::in);
	}

	bool Parser::Parse(Wave &wave)
	{
		if (!ParseHeader(wave.Header))
		{
			return false;
		}

		ChunkHeader chunkHeader;

		while (Stream.read(reinterpret_cast<char *>(&chunkHeader), sizeof(chunkHeader)))
		{
			std::string chunkId = MakeMarker(chunkHeader.SubChunkId);

			if (chunkId == "DATA")
			{
				wave.Data = std::make_shared<Chunk>(chunkHeader);
				if (!ParseChunk(*wave.Data))
				{
					return false;
				}
			}
			else if (chunkId == "LIST")
			{
				LISTChunk listChunk;
				if (!ParseList(listChunk, chunkHeader.SubChunkSize))
				{
					return false;
				}

				wave.List = std::move(listChunk);
			}
			else if (chunkId == "FMT ")
			{
				if (!ParseFmt(wave.Fmt))
				{
					return false;
				}
			}
			else if (chunkId == "ID3 ")
			{
				if (chunkHeader.SubChunkSize < sizeof(ID3Header))
					return false;

				auto chunkEnd = Stream.tellg() + (std::streamoff)chunkHeader.SubChunkSize;

				ID3Header id3{};
				if (!Stream.read(reinterpret_cast<char *>(&id3), sizeof(ID3Header)))
					return false;

				if (std::string(reinterpret_cast<char *>(id3.Identifier), 3) != "ID3")
					return false;

				const uint32_t payloadSize = chunkHeader.SubChunkSize - sizeof(ID3Header);
				if (utilities::DecodeSynchsafe(id3.Size) > payloadSize)
					return false;

				auto major = (uint32_t)id3.VersionMajor;
				auto revision = (uint32_t)id3.VersionRevision;
				auto &id3Chunk = wave.Id3Chunk;
				id3Chunk.Header = id3;

				printf("ID3 Version: v2.%x.%x\n", major, revision);
				printf("ID3 Size: %u\n", utilities::DecodeSynchsafe(id3.Size));
				printf("ID3 Flags: %s\n", ToString(id3.Flags).c_str());

				if (!ParseID3(id3Chunk, Version{2, major, revision}))
					return false;
				Stream.seekg(chunkEnd);
			}
			else
			{
				printf("Skipping Chunk... : %.4s (%u bytes)\n", chunkHeader.SubChunkId, chunkHeader.SubChunkSize);
				Stream.seekg(chunkHeader.SubChunkSize, std::ios::cur);
			}
		}

		return true;
	}

	bool Parser::ParseHeader(RIFFHeader &header)
	{
		Stream.read(reinterpret_cast<char *>(&header), sizeof(RIFFHeader));

		auto chunkId = MakeMarker(header.ChunkId);
		auto format = MakeMarker(header.Format);

		if (chunkId != "RIFF")
		{
			return false;
		}

		if (format != "WAVE")
		{
			return false;
		}

		return true;
	}

	bool Parser::ParseFmt(FMTChunk &fmtChunk)
	{
		if (!Stream.read((char *)&fmtChunk, sizeof(FMTChunk)))
			return false;
		return true;
	}

	bool Parser::ParseList(LISTChunk &listChunk, uint32_t size)
	{

		if (size < sizeof(listChunk.Type))
			return false;

		if (!Stream.read(reinterpret_cast<char *>(listChunk.Type), sizeof(listChunk.Type)))
			return false;

		const auto listEnd = Stream.tellg() + (std::streamoff)(size - sizeof(listChunk.Type));

		while (Stream.tellg() < listEnd)
		{
			if (listEnd - Stream.tellg() < sizeof(ChunkHeader))
				return false;

			ChunkHeader info{};
			if (!Stream.read(reinterpret_cast<char *>(&info), sizeof(ChunkHeader)))
				return false;

			info.SubChunkSize = utilities::FromLittleEndian(info.SubChunkSize);

			const auto dataEnd = Stream.tellg() + (std::streamoff)info.SubChunkSize;
			const auto paddedEnd = dataEnd + (std::streamoff)(info.SubChunkSize & 1);

			if (paddedEnd > listEnd)
				return false;

			Stream.seekg(dataEnd);

			if (!Stream)
				return false;

			Stream.seekg(paddedEnd);
			if (!Stream)
				return false;
		}
		return Stream.tellg() == listEnd;
	}

	bool Parser::ParseID3(ID3 &id3, const Version &version)
	{
		auto position = Stream.tellg();
		uint32_t tagSize = utilities::DecodeSynchsafe(id3.Header.Size);
		auto endPosition = position + (std::streamoff)tagSize;

		while (Stream.tellg() < endPosition)
		{
			std::string frameId(4, '\0');
			Stream.read(reinterpret_cast<char *>(frameId.data()), 4);

			if (frameId[0] == 0 && frameId[1] == 0 && frameId[2] == 0 && frameId[3] == 0)
				break;

			ID3FrameHeader header{};
			Stream.read(reinterpret_cast<char *>(&header), sizeof(ID3FrameHeader));

			header.Size = utilities::FromBigEndian(header.Size);
			header.Flags = (EFrameFlags)utilities::FromBigEndian(header.Flags);

			auto frame = ID3TagFactory::Get().CreateTag(version, frameId);

			if (frame)
			{
				auto start = Stream.tellg();

				frame->ProcessData(Stream, header);
				id3.Tags[std::hash<std::string>{}(frame->GetName())] = frame;

				uint32_t consumed = Stream.tellg() - start;
				printf("\t%s expected= %d consumed= %d\n", frameId.c_str(), header.Size, consumed);
			}
			else
			{
				Stream.seekg(header.Size, std::ios::cur);
			}
		}

		Stream.seekg(endPosition);
		return true;
	}

	bool Parser::ParseChunk(Chunk &chunk)
	{
		auto size = chunk.Header.SubChunkSize;
		if (!size)
			return false;

		chunk.Data.resize(size + 1);
		if (!Stream.read(reinterpret_cast<char *>(chunk.Data.data()), size))
			return false;

		return true;
	}

	std::string Parser::MakeMarker(const byte_t id[4])
	{
		std::string m(4, '\0');
		memcpy(m.data(), id, 4);
		std::transform(m.begin(), m.end(), m.begin(), ::toupper);
		return m;
	}
} // namespace waveparser