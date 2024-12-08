#include "frames/id3_Frame_Text.h"
#include "Utils.h"

namespace WAVE
{

	void id3_TextFrame_t::process_data(std::ifstream &stream)
	{
		stream.read((char *)&Impl, sizeof(Frame_IMPL));

		unsigned data_size = get_size();
		switch (Impl.encoding)
		{
		case ISO_8859_1:
		{
			data_size -= 1;
			text.resize(data_size + 1);
			break;
		}
		}

		stream.read(&text[0], data_size);
	}

	unsigned id3_TextFrame_t::get_size()
	{
		return utils::byte_array_to_unsigned(Impl.size);
	}
}