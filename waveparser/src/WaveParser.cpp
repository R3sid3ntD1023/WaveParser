#include "WaveParser.h"
#include "Version.h"
#include "id3_FrameHandler.h"
#include "utility/Utils.h"

namespace WAVE
{
	Parser::Parser(const std::filesystem::path &filename)
	{
		stream = std::ifstream(filename, std::ios::binary | std::ios::in);
	}

	bool Parser::parse(wave_t &wave)
	{
		if (!parse_header(wave.header))
		{
			return false;
		}

		ChunkInfo chunk_info;
		unsigned chunk_val = 0;

		while (stream.read(reinterpret_cast<char *>(&chunk_info), sizeof(ChunkInfo)))
		{
			chunk_val = utils::byte_array_to_int<uint32_t>(chunk_info.id);

			switch (chunk_val)
			{
			case DATA_MARKER:
			{
				wave.data = std::make_shared<Chunk>(chunk_info);
				parse_chunk(*wave.data);
				break;
			}
			case LIST_MARKER:
			{
				ListChunk list;
				stream.read(reinterpret_cast<char *>(list.type), 4);
				parse_list(list);

				wave.list = std::move(list);
				break;
			}
			case FMT_MARKER:
			{

				parse_fmt(wave.fmt);
				break;
			}
			default:
			{
				auto &chunk = *wave.extrachunks.emplace_back(std::make_shared<Chunk>(chunk_info));
				parse_chunk(chunk);
				break;
			}
			}
		};

		return true;
	}

	bool Parser::parse_header(WaveHeader &header)
	{
		stream.read(reinterpret_cast<char *>(&header), sizeof(WaveHeader));

		auto id = utils::byte_array_to_int<uint32_t>(header.id);
		auto format = utils::byte_array_to_int<uint32_t>(header.format);

		if (id != RIFF_TAG)
		{
			return false;
		}

		if (format != WAVE_TAG)
		{
			return false;
		}

		return true;
	}

	void Parser::parse_fmt(FMT_Chunk &fmt_chunck)
	{
		stream.read((char *)&fmt_chunck, sizeof(FMT_Chunk));
	}

	void Parser::parse_list(ListChunk &list_chunk)
	{
		ChunkInfo info{};

		while (stream.read(reinterpret_cast<char *>(&info), sizeof(ChunkInfo)))
		{
			info.size = utils::from_little_endian(info.size);

			std::cout << info.id << ":" << info.size << '\n';

			auto marker = utils::byte_array_to_int<uint32_t>(info.id);

			if (marker == ID3_MARKER)
			{
				ID3_Header id3{};
				stream.read(reinterpret_cast<char *>(&id3), sizeof(ID3_Header));

				auto major = (uint32_t)id3.versionMajor;
				auto revision = (uint32_t)id3.versionRevision;
				std::cout << "ID3 Version: v2." << std::hex << std::uppercase << major << "." << revision << '\n';

				auto &id3Chunck = list_chunk.id3_chunk;
				id3Chunck.header = id3;
				parse_id3(id3Chunck, Version{2, major, revision});
			}
			else
			{

				auto &chunk = *list_chunk.sub_chunks.emplace_back(std::make_unique<Chunk>(info));
				parse_chunk(chunk);
			}
		}
	}

	void Parser::parse_id3(ID3 &id3, const Version &version)
	{

		auto pos = stream.tellg();
		uint32_t tagSize = utils::decode_synh_safe(id3.header.size);
		auto endPos = pos + (std::streamoff)tagSize;

		while (stream.tellg() < endPos)
		{
			std::string frameID(4, '\0');
			stream.read(reinterpret_cast<char *>(frameID.data()), 4);

			if (frameID[0] == 0 && frameID[1] == 0 && frameID[2] == 0 && frameID[3] == 0)
				break;

			ID3FrameHeader header{};
			stream.read(reinterpret_cast<char *>(&header), sizeof(ID3FrameHeader));

			header.size = utils::from_big_endian(header.size);
			header.flags = (EFrameFlags)utils::from_big_endian(header.flags);

			auto frame = ID3TagFactory::Get().CreateTag(version, frameID);

			if (frame)
			{
				auto start = stream.tellg();

				frame->process_data(stream, header);
				id3.tags[std::hash<std::string>{}(frame->get_name())] = frame;

				uint32_t consumed = stream.tellg() - start;
				std::cout << frameID << " expected= " << tagSize << " consumed=" << consumed << '\n';
			}
			else
			{
				stream.seekg(header.size, std::ios::cur);
			}
		}

		stream.seekg(endPos);
	}

	void Parser::parse_chunk(Chunk &chunk)
	{
		auto size = chunk.header.size;
		if (size)
		{
			chunk.data = new byte_t[size + 1];
			stream.read(reinterpret_cast<char *>(chunk.data), size);
		}
	}
} // namespace WAVE