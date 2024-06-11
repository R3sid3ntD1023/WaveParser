#include "WaveData.h"


chunk_t::chunk_t(const chunk_info_t& info)
	:header(info)
{
	data.Allocate((size_t)info.size);
}

