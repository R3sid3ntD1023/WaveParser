#pragma once

#include "WaveData.h"
#include <iostream>

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
		Parser(const char *filename);

		bool parse(wave_t &wave);

	private:
		bool parse_header(wave_header_t &header);
		void parse_fmt(fmt_chunk_t &fmt_chunck);
		void parse_list(list_chunk_t &list_chunk);
		void parse_id3(id3_t &id3);
		void parse_chunk(chunk_t &chunk);

	private:
		std::ifstream stream;
	};
}