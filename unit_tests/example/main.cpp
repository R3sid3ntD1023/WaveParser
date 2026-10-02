#include "WaveParser.h"

#include <ShlObj.h>
#include <Windows.h>
#include <commdlg.h>
#include <conio.h>

#define AL_ALEXT_PROTOTYPES
#include <al.h>
#include <alc.h>
#include <alext.h>
#include <efx.h>

void InitializeOpenAL()
{
	ALCdevice *device = alcOpenDevice(nullptr);
	if (!device)
	{
		return;
	}

	ALCcontext *context = alcCreateContext(device, nullptr);
	if (!context)
	{
		alcCloseDevice(device);
		return;
	}

	alcMakeContextCurrent(context);
}

void CleanupOpenAL()
{
	ALCcontext *context = alcGetCurrentContext();
	if (context)
	{
		ALCdevice *device = alcGetContextsDevice(context);
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		if (device)
			alcCloseDevice(device);
	}
}

ALuint CreateBuffer(int16_t *data, size_t dataSize, ALuint format, ALuint sampleRate)
{
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, dataSize, sampleRate);

	return buffer;
}

ALuint CreateSource(ALuint buffer)
{
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	return source;
}

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
	if (!parsed)
		return 1;

	std::cout << "chunks:\n";
	for (auto &chunk : wave.List.SubChunks)
	{
		std::cout << "\t" << chunk->GetName() << " : ";
		std::cout.write(reinterpret_cast<const char *>(chunk->Data.data()), chunk->Data.size()) << "\n";
	}

	std::cout << "tags:\n";
	for (auto &tag : wave.Id3Chunk.GetTags())
	{
		std::cout << "\t" << "\n";
		std::cout << "\t" << ToString(tag->GetTagType()) << ": " << tag->ToString() << "\n";
	}

	std::cout << "length : " << wave.GetLength() << "\n";
	std::cout << "num samples : " << wave.GetNumSamplesPerChannel() << "\n";

	// std::cout.write(reinterpret_cast<const char *>(wave.GetData().data()), wave.GetData().size());

	InitializeOpenAL();

	ALenum audioFormat = 0;
	waveparser::FMTChunk &fmt = wave.Fmt;

	if (fmt.NumChannels == 1) // PMC
	{
		if (fmt.BitsPerSample == 16)
			audioFormat = AL_FORMAT_MONO16;
		else
			audioFormat = AL_FORMAT_MONO8;
	}
	else if (fmt.NumChannels == 2) // STEREO
	{
		if (fmt.BitsPerSample == 16)
			audioFormat = AL_FORMAT_STEREO16;
		else
			audioFormat = AL_FORMAT_STEREO8;
	}

	int16_t *audioData = reinterpret_cast<int16_t *>(wave.GetData().data());
	ALuint audioBuffer = CreateBuffer(audioData, wave.GetData().size(), audioFormat, wave.GetSampleRate());

	auto start = wave.Id3Chunk.GetTXXXByDescription("LOOP_START");
	auto end = wave.Id3Chunk.GetTXXXByDescription("LOOP_END");

	if (start.size() && end.size())
	{
		ALint loopStart = stoi(start[0]->GetValue());
		ALint loopEnd = stoi(end[0]->GetValue());
		ALint offsets[] = {loopStart, loopEnd};
		alBufferiv(audioBuffer, AL_LOOP_POINTS_SOFT, offsets);
	}

	ALuint audioSource = CreateSource(audioBuffer);
	alSourcei(audioSource, AL_LOOPING, AL_TRUE);

	alSourcePlay(audioSource);

	float playbackPos;
	bool run = true;

	while (run)
	{
		alGetSourcef(audioSource, AL_SEC_OFFSET, &playbackPos);
		printf("%.2f\r", playbackPos);
		fflush(stdout);

		if (_kbhit())
		{
			_getch();
			run = false;
		}

		Sleep(50);
	}
	printf("\n");

	alDeleteSources(1, &audioSource);
	alDeleteBuffers(1, &audioBuffer);
	CleanupOpenAL();

	return 0;
}