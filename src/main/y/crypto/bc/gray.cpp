#include "y/crypto/bc/gray.hpp"

namespace upsylon {

    namespace crypto {

#define Y_GRAY(N)       uint##N##_t
#define Y_GRAY_MAX(N) ( (4*sizeof(Y_GRAY(N)) ) )



#define Y_GRAY_ENCODE(N) \
template <> Y_GRAY(N) gray<Y_GRAY(N)>::encode( Y_GRAY(N) n ) throw() \
{\
return n ^ (n>>1);\
}

#if defined(__INTEL_COMPILER)
#pragma warning( push )
#pragma warning ( disable : 2259 )
#endif

        Y_GRAY_ENCODE(8)
        Y_GRAY_ENCODE(16)
        Y_GRAY_ENCODE(32)
        Y_GRAY_ENCODE(64)

#if 0
#define YOCTO_GRAY_DECODE(N) \
template <> Y_GRAY(N) gray<Y_GRAY(N)>::decode( Y_GRAY(N) n ) throw()\
{\
size_t        ish = 1;\
YOCTO_GRAY(N) ans = n;\
for(;;)\
{\
const YOCTO_GRAY(N) reg = ans>>ish;\
ans ^= reg;\
if (reg <= 1 || ish == YOCTO_GRAY_MAX(N) ) return ans;\
ish <<= 1;\
}\
}

        YOCTO_GRAY_DECODE(8)
        YOCTO_GRAY_DECODE(16)
        YOCTO_GRAY_DECODE(32)
        YOCTO_GRAY_DECODE(64)

#endif
        
    }
}
