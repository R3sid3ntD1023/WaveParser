#include "id3_FrameHandler.h"
#include "frames/v23/id3_Frame_TDRC.h"
#include "frames/v23/id3_Frame_COMM.h"
#include "frames/v23/id3_Frame_TXXX.h"
#include "frames/v23/id3_Frame_TIT2.h"
#include "frames/v23/id3_Frame_TPE1.h"

#define TPE1 0x54504531
#define TIT2 0x54495432
#define TDRC 0x54445243
#define COMM 0x434F4D4D
#define TXXX 0x54585858

namespace WAVE
{
	id3_registry::id3_registry()
	{
		Version v23(2, 3, 0);

		register_id3_tag<id3_Frame_COMM>(v23, COMM);
		register_id3_tag<id3_Frame_TDRC>(v23, TDRC);
		register_id3_tag<id3_Frame_TIT2>(v23, TIT2);
		register_id3_tag<id3_Frame_TPE1>(v23, TPE1);
		register_id3_tag<id3_Frame_TXXX>(v23, TXXX);
	}
}