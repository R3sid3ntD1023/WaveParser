#include "frames/v23/id3_Frame_TXXX.h"

namespace waveparser
{
	void ID3FrameTXXX::ProcessData(std::ifstream &stream, const ID3FrameHeader &header)
	{
		static_assert(sizeof(TXXXHeader) == 1);

		stream.read(reinterpret_cast<char *>(&Header), sizeof(Header));

		Text = ParseMultiString(stream, Header.Encoding, header.Size - sizeof(Header));
	}
} // namespace waveparser