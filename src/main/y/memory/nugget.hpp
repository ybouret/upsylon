//! \file
#ifndef Y_MEMORY_NUGGET_INCLUDED
#define Y_MEMORY_NUGGET_INCLUDED 1


#include "y/type/utils.hpp"
#include "y/os/static-check.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {

        //! handle memory for power of two aligned blocks
        template <const size_t BLOCK_BITS>
        class nugget
        {
        public:
            static const size_t block_bits = BLOCK_BITS;
            static const size_t block_size = 1 << BLOCK_BITS;

            const size_t   provided_number;  //!<
            size_t         still_available;  //!< for bookeeping
            size_t         first_available;  //!< for bookeeping
            uint8_t       *data;
            const uint8_t *last;
            nugget        *next;
            nugget        *prev;
            const size_t   bytes;

            inline nugget(const size_t chunk_size,
                          void        *user_entry) throw() :
            provided_number( chunk_size / block_size ),
            still_available( provided_number ),
            first_available( 0 ),
            data( static_cast<uint8_t *>(user_entry) ),
            last( data+chunk_size ),
            next(0),
            prev(0),
            bytes(chunk_size)
            {
                Y_STATIC_CHECK(block_size>=sizeof(size_t),block_bits_too_small);
                assert(user_entry);
                // format
                uint8_t *p = data;
                for(size_t i=0; i != provided_number; p += block_size )
                {
                    *(size_t *)p = ++i;
                }

            }

            inline ~nugget() throw()
            {
            }

            inline void *acquire() throw()
            {
                assert(still_available>0);
                uint8_t *p = &data[ first_available << BLOCK_BITS ];
                first_available = *(size_t*)p;
                --still_available;
                memset(p,0,block_size);
                return p;
            }

            inline void release(void *p) throw()
            {
                uint8_t *to_release = static_cast<uint8_t*>(p);
                assert(to_release>=data);
                assert(to_release <last);
                assert( (to_release-data) % block_size == 0 );
                *(size_t *)to_release = first_available;
                first_available = static_cast<size_t>(to_release-data) >> BLOCK_BITS;
                ++still_available;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(nugget);
        };

    }
}

#endif
