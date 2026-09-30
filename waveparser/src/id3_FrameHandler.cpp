#include "id3_FrameHandler.h"
#include "frames/v23/id3_Frame_COMM.h"
#include "frames/v23/id3_Frame_TDRC.h"
#include "frames/v23/id3_Frame_TIT2.h"
#include "frames/v23/id3_Frame_TPE1.h"
#include "frames/v23/id3_Frame_TXXX.h"

namespace WAVE
{
	ID3TagFactory::ID3TagFactory()
	{
		Version v23(2, 3, 0);

		Register<ID3_Frame_COMM>(v23, "COMM");
		Register<ID3_Frame_TDRC>(v23, "TDRC");
		Register<ID3_Frame_TIT2>(v23, "TIT2");
		Register<ID3_Frame_TPE1>(v23, "TPE1");
		Register<ID3_Frame_TXXX>(v23, "TXXX");
	}
} // namespace WAVE