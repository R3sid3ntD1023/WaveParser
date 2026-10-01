#pragma once

#include "id3_Frame_Text.h"

namespace waveparser
{
	struct ID3FrameTDRC : public ID3TextFrame
	{

		ETagType GetTagType() const override { return ETagType::TDRC; }

		std::string GetDescription() const { return "RecordingTime"; }
	};
} // namespace waveparser