
#include "y/ios/scribe/native.hpp"

namespace upsylon
{
    namespace ios
    {

        native_scribe:: ~native_scribe() throw()
        {
        }
        
        native_scribe:: native_scribe(const std::type_info &tid,
                                      const size_t          tsz,
                                      const char *how) :
        scribe1D(tid,tsz),
        fmt(how)
        {
        }
        
    }
    
}

