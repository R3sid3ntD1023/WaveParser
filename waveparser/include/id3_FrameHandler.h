#pragma once

#include "Core.h"
#include "frames/id3_Frame.h"

struct id3_FrameHandler
{
	static id3_frame_ptr process(FILE* file, unsigned tag_id);
};