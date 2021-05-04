

#include "y/ios/scribe.hpp"


namespace upsylon
{
    namespace ios
    {
        scribe:: ~scribe() throw()
        {}
        
        scribe:: scribe(const std::type_info &tid,
                        const size_t          tsz,
                        const size_t          num) :
        info( rtti::of(tid) ),
        size( tsz ),
        dims(num)
        {
            assert( 0==(size%dims) );
        }
        
        const   rtti  &scribe:: key() const throw()
        {
            return info;
        }

        
    }

}

