#include "WaveParser.h"

#include <ShlObj.h>
#include <Windows.h>
#include <commdlg.h>

int main(int argv, char **argc)
{

	std::string filePath = "";

	OPENFILENAMEA fileDialog;
	CHAR selectedFile[260] = {0};
	CHAR currentDirectory[256] = {0};
	ZeroMemory(&fileDialog, sizeof(OPENFILENAME));
	fileDialog.lStructSize = sizeof(OPENFILENAME);
	fileDialog.hwndOwner = nullptr;
	fileDialog.lpstrFile = selectedFile;
	fileDialog.nMaxFile = sizeof(selectedFile);
	if (GetCurrentDirectoryA(256, currentDirectory))
		fileDialog.lpstrInitialDir = currentDirectory;
	fileDialog.lpstrFilter = "(wave)\0*.wav\0";
	fileDialog.nFilterIndex = 1;
	fileDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&fileDialog) == TRUE)
		filePath = fileDialog.lpstrFile;

	waveparser::Parser parser(filePath);

	waveparser::Wave wave{};

	bool parsed = parser.Parse(wave);
	if (parsed)
	{
		std::cout << "chunks:\n";
		for (auto &chunk : wave.List.SubChunks)
		{
			std::cout << "\t" << chunk->GetName() << " : ";
			std::cout.write(reinterpret_cast<const char *>(chunk->Data.data()), chunk->Data.size()) << "\n";
		}

		std::cout << "tags:\n";
		for (auto &tag : wave.Id3Chunk.GetTags())
		{
			std::cout << "\t" << tag->GetName() << ": " << tag->ToString() << "\n";
		}

		std::cout << "length : " << wave.GetLength() << "\n";
		std::cout << "num samples : " << wave.GetNumSamplesPerChannel() << "\n";

		// std::cout.write(reinterpret_cast<const char *>(wave.GetData().data()), wave.GetData().size());
	}

	return 0;
}