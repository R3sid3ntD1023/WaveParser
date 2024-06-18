#pragma once

#include "Core.h"
#include "Utils.h"



enum id3_flag : byte
{
	Unsynchronisation = 0,
	ExtentedHeader = 1,
	ExperimentalIndictor = 2,
	FooterPresent = 3
};

struct id3_Frame_t
{
	enum e_frame_flags : byte
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
	virtual void print() = 0;
	virtual void process_data(FILE* f) = 0;
	virtual std::string get_name() const = 0;
	virtual std::string get_description() const = 0;
};

typedef std::shared_ptr<id3_Frame_t> id3_frame_ptr;







