#pragma once

#include "Core.h"

namespace waveparser
{
	class Version
	{
	private:
		uint32_t Major;
		uint32_t Minor;
		uint32_t Revision;

	public:
		Version(uint32_t major, uint32_t minor, uint32_t revision)
			: Major(major),
			  Minor(minor),
			  Revision(revision)
		{
		}

		bool operator==(const Version &rhs) const { return Major == rhs.Major && Minor == rhs.Minor && Revision == rhs.Revision; }

		const std::string ToString() const
		{
			std::stringstream ss;
			ss << Major << "." << Minor << "." << Revision;
			return ss.str();
		}

		friend struct std::hash<Version>;
	};

} // namespace waveparser

namespace std
{
	template <>
	struct hash<waveparser::Version>
	{
		size_t operator()(const waveparser::Version &version) const
		{
			return hash<uint32_t>()(version.Major) | hash<uint32_t>()(version.Minor) | hash<uint32_t>()(version.Revision);
		}
	};

} // namespace std
