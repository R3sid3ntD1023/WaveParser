#pragma once

#include "Core.h"
#include "Version.h"
#include "id3_Frame.h"

namespace waveparser
{

	class ID3TagFactory
	{
		using CreateTagFunction = std::function<std::shared_ptr<ID3Frame>()>;

	public:
		ID3TagFactory();

		template <typename T>
		void Register(const Version &version, ETagType type)
		{
			auto createFunc = []() { return std::make_shared<T>(); };
			Tags[version].emplace(type, std::move(createFunc));
		}

		std::shared_ptr<ID3Frame> CreateTag(const Version &version, ETagType type)
		{
			if (!Tags[version].contains(type))
				return nullptr;

			return Tags[version].at(type)();
		}

		static ID3TagFactory &Get()
		{
			static ID3TagFactory registry;
			return registry;
		}

	private:
		std::unordered_map<Version, std::unordered_map<ETagType, CreateTagFunction>> Tags;
	};
} // namespace waveparser
