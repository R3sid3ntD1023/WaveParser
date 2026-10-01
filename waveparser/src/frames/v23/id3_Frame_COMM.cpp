#include "frames/v23/id3_Frame_COMM.h"

namespace waveparser
{
	void ID3FrameCOMM::ProcessData(std::ifstream &stream, const ID3FrameHeader &header)
	{
		static_assert(sizeof(COMMHeader) == 4);

		stream.read(reinterpret_cast<char *>(&Header), sizeof(Header));

		Text = ParseMultiString(stream, Header.Encoding, header.Size - sizeof(Header));
	}
} // namespace waveparser