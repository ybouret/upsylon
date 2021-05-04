#include "y/ios/scribe/in1d.hpp"

namespace upsylon
{
    namespace ios
    {
        scribe1D:: ~scribe1D() throw()
        {}

        scribe1D:: scribe1D(const std::type_info &tid) :
        scribe(tid,1)
        {
        }


    }

}


