#include "WaveParser.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE	"\x1b[37m"

void LogCallback(Logger::LogLevel level, const char* message)
{
	switch (level)
	{
	case Logger::info:
		printf(ANSI_COLOR_GREEN"%s\n", message);
		break;
	case Logger::trace:
		printf(ANSI_COLOR_WHITE"%s\n", message);
		break;
	case Logger::warn:
		printf(ANSI_COLOR_YELLOW"%s\n", message);
		break;
	case Logger::error:
		printf(ANSI_COLOR_RED"%s\n", message);
		break;
	default:
		break;
	}

	printf(ANSI_COLOR_WHITE"");
}

int main(int argv, char** argc)
{
	Logger::SetCallback(LogCallback);

	WaveParser parser(RESOURCES_PATH"Resident Evil 5 - 'Rust in Summer 2008' (Versus Mode - Slayers).wav");
	auto wave = parser.parse();

	WAVE_LOG(info, "chunks:");
	for (auto& [name, chunk] : wave.list.sub_chunks)
	{
		WAVE_LOG(info, "\t{}", chunk->get_name());
	}

	WAVE_LOG(info, "tags:");
	for (auto& [name, frame] : wave.list.id3_chunk.tags)
	{
		WAVE_LOG(info ,"\t{}", frame->get_name());
	}

	WAVE_LOG(info, "length :{}", wave.get_length());
	WAVE_LOG(info, "num samples : {}", wave.get_num_samples_per_channel());
	WAVE_LOG(info, "buffer size : {}", wave.get_buffer_size());
	
	return 0;
}