

#include "y/yap/natural.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon {

    namespace yap {

        const char natural:: CLASS_NAME[] = "yapn";

        const char * natural:: className()       const throw() { return CLASS_NAME; }

        size_t natural:: serialize(ios::ostream &fp) const
        {
            assert( check(*this,"self@serialize") );
            const size_t ans = fp.write_upack(bytes);
            for(size_t i=0;i<bytes;++i)
            {
                fp.write( get(i) );
            }
            return ans+bytes;
        }

    }

}

