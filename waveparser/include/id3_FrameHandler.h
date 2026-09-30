#pragma once

#include "Core.h"
#include "Version.h"
#include "id3_Frame.h"

namespace WAVE
{

	class ID3TagFactory
	{
		using CreateTagFuncSigniture = std::function<std::shared_ptr<ID3_Frame_T>()>;

	public:
		ID3TagFactory();

		template <typename T>
		void Register(const Version &version, const std::string &id)
		{
			auto hash = std::hash<std::string>{}(id);
			if (!mTags[version].contains(hash))
			{
				auto create_func = []() { return std::make_shared<T>(); };
				mTags[version].emplace(hash, std::move(create_func));
			}
		}

		std::shared_ptr<ID3_Frame_T> CreateTag(const Version &version, const std::string &id)
		{
			auto hash = std::hash<std::string>{}(id);
			if (!mTags[version].contains(hash))
				return nullptr;

			return mTags[version].at(hash)();
		}

		static ID3TagFactory &Get()
		{
			static ID3TagFactory registry;
			return registry;
		}

	private:
		std::unordered_map<Version, std::unordered_map<uint64_t, CreateTagFuncSigniture>> mTags;
	};
} // namespace WAVE
