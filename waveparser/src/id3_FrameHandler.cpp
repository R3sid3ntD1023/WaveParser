#include "id3_FrameHandler.h"
#include "id3_Frames.h"

#define make_frame(cls) std::make_shared<cls>();

id3_frame_ptr id3_FrameHandler::process(FILE* file, unsigned tag_id)
{
	id3_frame_ptr frame = nullptr;
	switch (tag_id)
	{
	case COMM:
	{
		frame = make_frame(id3_Frame_COMM);
		break;
	}
	case TIT2:
	{
		frame = make_frame(id3_Frame_TIT2);
		break;
	}
	case TDRC:
	{
		frame = make_frame(id3_Frame_TDRC);
		break;
	}
	case TXXX:
	{
		frame = make_frame(id3_Frame_TXXX);
		break;
	}
	case TPE1:
	{
		frame = make_frame(id3_Frame_TPE1);
		break;
	}
	default:
		break;
	}

	if(frame) frame->process_data(file);

	return std::move(frame);
}
