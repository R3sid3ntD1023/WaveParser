#pragma once

#include "Core.h"
#include "id3_Frame.h"
#include "Version.h"

namespace WAVE
{

	class id3_registry
	{
	public:
		id3_registry();

		template <typename T>
		void register_id3_tag(const Version &version, unsigned id)
		{
			if (_registered_id3_tags[version].contains(id))
				return;

			_registered_id3_tags[version].emplace(id, std::make_shared<T>());
		}

		id3_frame_ptr get_id3_tag(const Version &version, unsigned id)
		{
			if (_registered_id3_tags[version].contains(id))
				return _registered_id3_tags[version].at(id);

			return nullptr;
		}

		static id3_registry &get()
		{
			static id3_registry registry;
			return registry;
		}

	private:
		std::unordered_map<Version, std::unordered_map<unsigned, id3_frame_ptr>> _registered_id3_tags;
	};
}
