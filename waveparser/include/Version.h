#pragma once

#include "Core.h"
#include <sstream>

namespace WAVE
{
    class Version
    {
    private:
        /* data */
    public:
        Version(unsigned major, unsigned minor, unsigned revision)
            : mMajor(major), mMinor(minor), mRevision(revision)
        {
        }

        bool operator==(const Version &rhs) const { return mMajor == rhs.mMajor && mMinor == rhs.mMinor && mRevision == rhs.mRevision; }
        bool operator!=(const Version &rhs) const { return !(*this == rhs); }

        const std::string to_string() const
        {
            std::stringstream ss;
            ss << mMajor << "." << mMinor << "." << mRevision;
            return ss.str();
        }

    private:
        unsigned mMajor;
        unsigned mMinor;
        unsigned mRevision;

        friend struct std::hash<Version>;
    };

} // namespace BHive

namespace std
{
    template <>
    struct hash<WAVE::Version>
    {
        size_t operator()(const WAVE::Version &version) const
        {
            return hash<unsigned>()(version.mMajor) | hash<unsigned>()(version.mMinor) |
                   hash<unsigned>()(version.mRevision);
        }
    };

} // namespace std
