#include "y/crypto/sc/kstream.hpp"

namespace upsylon {

    namespace crypto
    {
        kstream:: ~kstream() throw() {}

        kstream:: kstream() throw()
        {

        }

        uint8_t kstream:: operator()(const uint8_t B) throw()
        {
            return (B^call());
        }


        void kstream:: operator()( void *target, const void *source, const size_t length) throw()
        {
            assert( !(0==target&&length>0) );
            assert( !(0==source&&length>0) );
            const uint8_t *p = static_cast<const uint8_t *>(source);
            uint8_t       *q = static_cast<uint8_t       *>(target);
            for(size_t i=length;i>0;--i)
            {
                *(q++) = ( *(p++) ^ call() );
            }
        }

    }
}

