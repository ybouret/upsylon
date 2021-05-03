

#include "y/ios/scribe/tuple.hpp"

namespace upsylon
{
    namespace ios
    {
        scribeND:: ~scribeND() throw()
        {}

        scribeND:: scribeND(const std::type_info &tid,
                            const scribe1D       &one,
                            const size_t          num) :
        scribe(tid,one.size*num,num),
        base(one)
        {
        }

        string scribeND:: write(const void *addr) const throw()
        {
            const  uint8_t *ptr = static_cast<const uint8_t *>(addr);
            string          ans = base.write(ptr);
            for(size_t i=1;i<dims;++i)
            {
                ans << ' ' << base.write(ptr+=base.size);
            }
            return ans;
        }

    }

}


