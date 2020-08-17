

#include "y/yap/natural.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/exception.hpp"

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

        natural natural:: read(ios::istream &fp, size_t &delta, const char *which)
        {
            static const char fn[] = "yap::natural::read";
            assert(which);
            size_t num_bytes = 0;
            if( !fp.query_upack(num_bytes,delta) ) throw exception("%s missing #bytes for '%s'",fn,which);
            natural n(num_bytes,as_capacity);
            for(size_t i=0;i<num_bytes;++i)
            {
                char C = 0;
                if(!fp.query(C)) throw exception("%s missing byte[%lu] for '%s'",fn,(unsigned long)i,which);
                n.get(i) = uint8_t(C);
                ++delta;
            }
            n.upgrade();
            if(n.bytes!=num_bytes) throw exception("%s corrupted '%s'", fn, which);
            return n;
        }


    }

}

