#pragma once

#include "Core.h"
#include "id3_Frame.h"

namespace WAVE
{

	class id3_registry
	{
	public:
		id3_registry();

		template <typename T>
		void register_id3_tag(unsigned id)
		{
			if (_registered_id3_tags.contains(id))
				return;

			_registered_id3_tags.emplace(id, std::make_shared<T>());
		}

		id3_frame_ptr get_id3_tag(unsigned id)
		{
			if (_registered_id3_tags.contains(id))
				return _registered_id3_tags.at(id);

			return nullptr;
		}

		static id3_registry &get()
		{
			static id3_registry registry;
			return registry;
		}

	private:
		std::unordered_map<unsigned, id3_frame_ptr> _registered_id3_tags;
	};
}
