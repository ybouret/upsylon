#include "y/ios/osstream.hpp"

namespace upsylon
{
    namespace ios
    {
        void osstream:: write(char C) { host << C; }
        void osstream:: flush() {}

        osstream:: ~osstream() throw() {}
        osstream:: osstream( string &s ) throw() : host(s) {}

        embedded_osstream:: embedded_osstream() : string(), osstream( static_cast<string&>(*this) )
        { }

        embedded_osstream:: ~embedded_osstream() throw() {}

    }

}


