#include "WaveParser.h"

#include <ShlObj.h>
#include <Windows.h>
#include <commdlg.h>

int main(int argv, char **argc)
{

	std::string filename = "";

	OPENFILENAMEA ofn;
	CHAR szFile[260] = {0};
	CHAR currentDir[256] = {0};
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = "(wave)\0*.wav\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		filename = ofn.lpstrFile;

	WAVE::Parser parser(filename);

	WAVE::wave_t wave{};

	if (parser.parse(wave))
	{
		std::cout << "chunks:\n";
		for (auto &chunk : wave.list.sub_chunks)
		{
			std::cout << "\t" << chunk->get_name() << " : " << chunk->get_data() << "\n";
		}

		std::cout << "tags:\n";
		for (auto &[name, frame] : wave.list.id3_chunk.get_tags())
		{
			std::cout << "\t" << frame->to_string() << "\n";
		}

		std::cout << "length : " << wave.get_length() << "\n";
		std::cout << "num samples : " << wave.get_num_samples_per_channel() << "\n";
		std::cout << "buffer size : " << wave.get_buffer_size() << "\n";

		std::cout << "Data : " << wave.data->get_name() << "\n";
	}

	return 0;
}