#pragma once

#include "Version.h"
#include "WaveData.h"

namespace waveparser
{
	class Parser
	{
	public:
		Parser(const std::filesystem::path &filePath);

		bool Parse(Wave &wave);

	private:
		bool ParseHeader(RIFFHeader &header);
		bool ParseFmt(FMTChunk &fmtChunk);
		bool ParseList(LISTChunk &listChunk, uint32_t size);
		bool ParseID3(ID3 &id3, const Version &version);
		bool ParseChunk(Chunk &chunk);
		std::string MakeMarker(const byte_t id[4]);

	private:
		std::ifstream Stream;
	};
} // namespace waveparser