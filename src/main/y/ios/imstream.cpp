
#include "y/ios/imstream.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace ios
    {
        void imstream:: store(const char C)
        {
            static const char what[] = "imstream store failure";
            if(curr<=init)
            {
                throw imported::exception(what,"impossible at stream start");
            }
            if(curr[-1]!=C)
            {
                throw imported::exception(what,"mismatching characters");
            }
            --curr;
        }
    }
}

