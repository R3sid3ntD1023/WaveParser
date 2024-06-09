#include "frames/id3_Frame_Text.h"

void id3_TextFrame_t::process_data(FILE* f)
{
	fread(&Impl, sizeof(Frame_IMPL), 1, f);

	unsigned data_size =  get_size();
	switch (Impl.encoding)
	{
		case ISO_8859_1: 
		{ 
			data_size -= 1;
			text.resize(data_size + 1);
			break; 
		}
	}

	fread(&text[0], sizeof(char), data_size, f);

}

unsigned id3_TextFrame_t::get_size()
{
	return utils::byte_array_to_int(Impl.size);
}

void id3_TextFrame_t::print()
{
	auto _size = get_size();
	printf(" %s\n", text.c_str());
}