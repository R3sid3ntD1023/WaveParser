#pragma once

#include "Core.h"
#include "Utils.h"

namespace WAVE
{

	enum id3_flag : byte_t
	{
		id3_flag_None = 0,
		id3_flag_Unsynchronisation = 1 << 0,
		id3_flag_ExtentedHeader = 1 << 1,
		id3_flag_ExperimentalIndictor = 1 << 2,
		id3_flag_FooterPresent = 1 << 3
	};

	struct id3_Frame_t
	{
		enum e_frame_flags : byte_t
		{
			TagAlterPreservation = 0,
			FileAlterPreservation,
			ReadOnly,
			Compression,
			Encryption,
			GroupingIdentity
		};

		e_frame_flags flag;

		virtual ~id3_Frame_t() = default;
		virtual void process_data(std::ifstream &stream) = 0;
		virtual std::string get_name() const = 0;
		virtual std::string get_description() const = 0;
		virtual std::string to_string() const = 0;
	};

	typedef std::shared_ptr<id3_Frame_t> id3_frame_ptr;

} // namespace WAVE
