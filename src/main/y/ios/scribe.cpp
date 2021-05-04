#include "y/ios/scribe.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        const char * const scribe:: CLSID = "ios::scribe";

        scribe:: ~scribe() throw()
        {}
        
        scribe:: scribe(const std::type_info &tid,
                        const size_t          num) :
        info( rtti::of(tid) ),
        dims(num)
        {
            if(info.size<=0)            throw exception("%s(unset <%s>.size)",CLSID,info.text());
            if( 0 != (info.size%dims) ) throw exception("%s(<%s>.size=%u isn't a multiple of %u)",CLSID,info.text(),unsigned(info.size),unsigned(dims));
        }
        
        const rtti  &scribe:: key() const throw()
        {
            return info;
        }

        
    }

}

