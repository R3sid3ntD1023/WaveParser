#include "id3_FrameHandler.h"
#include "frames/v23/id3_Frame_COMM.h"
#include "frames/v23/id3_Frame_TDRC.h"
#include "frames/v23/id3_Frame_TIT2.h"
#include "frames/v23/id3_Frame_TPE1.h"
#include "frames/v23/id3_Frame_TXXX.h"

namespace waveparser
{
	ID3TagFactory::ID3TagFactory()
	{
		Version versionV23(2, 3, 0);

		Register<ID3FrameCOMM>(versionV23, ETagType::COMM);
		Register<ID3FrameTDRC>(versionV23, ETagType::TDRC);
		Register<ID3FrameTIT2>(versionV23, ETagType::TIT2);
		Register<ID3FrameTPE1>(versionV23, ETagType::TPE1);
		Register<ID3FrameTXXX>(versionV23, ETagType::TXXX);
	}
} // namespace waveparser