
#include "y/yap/natural.hpp"
#include <iostream>

namespace upsylon {

    namespace yap {

#define Y_APN_CHECK(EXPR) do { \
/**/ if(!(EXPR))\
/**/     {\
/**/         std::cerr <<  pfx << which << mid << #EXPR << sfx  << std::endl;\
/**/         return false;\
/**/     }\
/**/ } while(false)

        bool natural:: check(const natural &n, const char *which) throw()
        {
            assert(which);
            static const char pfx[] = "***[natural] '";
            static const char mid[] = "' : ";
            static const char sfx[] = " FAILURE!";

            Y_APN_CHECK(n.words==words_for(n.bytes));
            Y_APN_CHECK(n.count>=n.words);
            Y_APN_CHECK(n.count*word_size==n.width);
            Y_APN_CHECK(n.width == (size_t(1) << n.shift) );
            Y_APN_CHECK(n.bytes <= n.width );
            if(n.bytes>0)
            {
                Y_APN_CHECK(n.get(n.bytes-1)>0);
            }
            for(size_t remaining=n.bytes;remaining<n.width;++remaining)
            {
                Y_APN_CHECK(0==n.get(remaining));
            }
            return true;
        }

    }
}


