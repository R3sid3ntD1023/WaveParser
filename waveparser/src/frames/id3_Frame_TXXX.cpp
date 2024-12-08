#include "frames/id3_Frame_TXXX.h"

namespace WAVE
{

	id3_Frame_TXXX::~id3_Frame_TXXX()
	{
	}

	void id3_Frame_TXXX::process_data(std::ifstream &stream)
	{
		stream.read((char *)&DescritpionImpl, sizeof(Frame_IMPL));
		auto size = utils::byte_array_to_unsigned(DescritpionImpl.size);

		switch (DescritpionImpl.encoding)
		{
		case ISO_8859_1:
		{
			size -= 1;
			break;
		}
		}

		std::string text;
		text.resize(size);
		stream.read(&text[0], size);

		auto delimiter = '\0';
		size_t pos = text.find(delimiter);
		Desciption = text.substr(0, pos);
		Value = text.substr(pos + 1);
	}

	std::string WAVE::id3_Frame_TXXX::to_string() const
	{
		return Desciption + " : " + Value;
	}

}
