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
		void Register(const Version &version, const std::string &id)
		{
			auto hash = std::hash<std::string>{}(id);
			if (!Tags[version].contains(hash))
			{
				auto createFunc = []() { return std::make_shared<T>(); };
				Tags[version].emplace(hash, std::move(createFunc));
			}
		}

		std::shared_ptr<ID3Frame> CreateTag(const Version &version, const std::string &id)
		{
			auto hash = std::hash<std::string>{}(id);
			if (!Tags[version].contains(hash))
				return nullptr;

			return Tags[version].at(hash)();
		}

		static ID3TagFactory &Get()
		{
			static ID3TagFactory registry;
			return registry;
		}

	private:
		std::unordered_map<Version, std::unordered_map<uint64_t, CreateTagFunction>> Tags;
	};
} // namespace waveparser
