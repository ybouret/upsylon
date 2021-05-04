

#include "y/ios/scribe.hpp"


namespace upsylon
{
    namespace ios
    {
        scribe:: ~scribe() throw()
        {}
        
        scribe:: scribe(const std::type_info &tid,
                        const size_t          num) :
        info( rtti::of(tid) ),
        dims(num)
        {
            assert( 0==(info.size%dims) );
        }
        
        const rtti  &scribe:: key() const throw()
        {
            return info;
        }

        
    }

}

