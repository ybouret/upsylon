
#include "y/associative/suffix/xaddress.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{

    namespace core
    {
        const void * suffix_xaddr :: be(const void *addr) throw()
        {
            return swap_be_as(addr);
        }

        void suffix_xaddr:: be(void *target, const void *source, size_t bytes) throw()
        {
            assert(bytes>0);
            assert(target);
            assert(source);

#if Y_BYTE_ORDER == Y_BIG_ENDIAN
            memcpy(target,source,bytes);
#endif

#if Y_BYTE_ORDER == Y_LIT_ENDIAN
            const uint8_t *src = static_cast<const uint8_t*>(source) + bytes;
            uint8_t       *tgt = static_cast<uint8_t      *>(target);
            while(bytes-- > 0)
            {
                *(tgt++) = *(--src);
            }
#endif
        }

    }

}
