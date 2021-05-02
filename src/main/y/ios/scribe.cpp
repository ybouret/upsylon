

#include "y/ios/scribe.hpp"


namespace upsylon
{
    namespace ios
    {
        scribe:: ~scribe() throw()
        {}
        
        scribe:: scribe(const std::type_info &tid) :
        info( rtti::of(tid) )
        {
            
        }
        
        
        
    }

}

