#pragma once

#include "Version.h"
#include "WaveData.h"

#define DATA_MARKER 0x64617461
#define LIST_MARKER 0x4C495354
#define FMT_MARKER 0x666D7420
#define RIFF_TAG 0x52494646
#define WAVE_TAG 0x57415645
#define ID3_MARKER 0x69643320

namespace WAVE
{

	class Parser
	{
	public:
		Parser(const std::filesystem::path &filename);

		bool parse(wave_t &wave);

	private:
		bool parse_header(WaveHeader &header);
		void parse_fmt(FMT_Chunk &fmt_chunck);
		void parse_list(ListChunk &list_chunk);
		void parse_id3(ID3 &id3, const Version &version);
		void parse_chunk(Chunk &chunk);

	private:
		std::ifstream stream;
	};
}