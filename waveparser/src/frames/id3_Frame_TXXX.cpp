#include "frames/id3_Frame_TXXX.h"

id3_Frame_TXXX::~id3_Frame_TXXX()
{
	
}

void id3_Frame_TXXX::process_data(FILE* f)
{
	fread(&DescritpionImpl, sizeof(Frame_IMPL), 1, f);
	auto descriptionSize = utils::byte_array_to_int(DescritpionImpl.size);

	switch (DescritpionImpl.encoding)
	{
		case ISO_8859_1:
		{
			descriptionSize -= 1;
			break;
		}
	}

	std::string text;
	text.resize(descriptionSize);
	fread(&text[0], sizeof(byte_t), descriptionSize, f);

	auto delimiter = '\0';
	size_t pos = text.find(delimiter);
	Desciption = text.substr(0, pos);
	Value = text.substr(pos + 1);
}

void id3_Frame_TXXX::print()
{
	WAVE_LOG(info ,"{} : {}",  Desciption.c_str(), Value.c_str());
}