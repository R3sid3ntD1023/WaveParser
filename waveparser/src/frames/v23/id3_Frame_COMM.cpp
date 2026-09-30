#include "frames/v23/id3_Frame_COMM.h"

namespace WAVE
{
	void ID3_Frame_COMM::process_data(std::ifstream &stream, const ID3FrameHeader &h)
	{
		static_assert(sizeof(Header) == 4);

		stream.read(reinterpret_cast<char *>(&Header), sizeof(Header));

		Text = parseMultiString(stream, Header.encoding, h.size - sizeof(Header));
	}
} // namespace WAVE