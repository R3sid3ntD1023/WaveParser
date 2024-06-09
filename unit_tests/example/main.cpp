#include "WaveParser.h"

int main(int argv, char** argc)
{
	WaveParser parser(RESOURCES_PATH"Resident Evil 5 - 'Rust in Summer 2008' (Versus Mode - Slayers).wav");
	auto wave = parser.parse();

	printf("chunks: \n");
	for (auto& [name, chunk] : wave.list.sub_chunks)
	{
		printf("\t%.*s\n", 4, chunk->get_name());
	}

	printf("tags: \n");
	for (auto& [name, frame] : wave.list.id3_chunk.tags)
	{
		printf("\t%s \n", frame->get_name());
	}

	printf("length : %f\n", wave.get_length());
	printf("num samples : %d\n", wave.get_num_samples_per_channel());
	printf("buffer size : %d\n", wave.get_buffer_size());
	
	return 0;
}