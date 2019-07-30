
#include "y/lang/pattern/vfs-matching.hpp"

namespace upsylon
{
    namespace Lang
    {
        MatchVFS:: ~MatchVFS() throw() {}

        MatchVFS:: MatchVFS( const string &rx, const Mode m) : MatchString(rx), mode(m), verbose(false)  {}

        MatchVFS:: MatchVFS( const char   *rx, const Mode m) : MatchString(rx), mode(m), verbose(false) {}

        MatchVFS:: MatchVFS( const MatchVFS &other ) : MatchString(other), mode(other.mode), verbose(other.verbose) {}

        bool MatchVFS:: operator()(const vfs::entry &e)
        {
            MatchString &self = *this;
            bool         flag = false;
            switch(mode)
            {
                case ByPath: flag = self(e.path);      break;
                case ByName: flag = self(e.base_name); break;
            }
            if(flag && verbose)
            {
                std::cerr << "[MatchVFS] '" << self << "'" << std::endl;
            }
            return flag;
        }

    }
}
