#pragma once

#include "Core.h"

namespace waveparser
{
	class Version
	{
	private:
		/* data */
	public:
		Version(unsigned major, unsigned minor, unsigned revision)
			: Major(major),
			  Minor(minor),
			  Revision(revision)
		{
		}

		bool operator==(const Version &rhs) const { return Major == rhs.Major && Minor == rhs.Minor && Revision == rhs.Revision; }
		bool operator!=(const Version &rhs) const { return !(*this == rhs); }

		const std::string ToString() const
		{
			std::stringstream ss;
			ss << Major << "." << Minor << "." << Revision;
			return ss.str();
		}

	private:
		unsigned Major;
		unsigned Minor;
		unsigned Revision;

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
			return hash<unsigned>()(version.Major) | hash<unsigned>()(version.Minor) | hash<unsigned>()(version.Revision);
		}
	};

} // namespace std
