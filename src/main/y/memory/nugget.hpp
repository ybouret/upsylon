
#ifndef Y_MEMORY_NUGGET_INCLUDED
#define Y_MEMORY_NUGGET_INCLUDED 1


#include "y/type/utils.hpp"
#include "y/os/static-check.hpp"
#include <iostream>
#include <cstring>
namespace upsylon
{
    namespace memory
    {

        template <const size_t BLOCK_BITS>
        class nugget
        {
        public:
            static const size_t BLOCK_SIZE = 1 << BLOCK_BITS;
            const size_t   provided_number;  //!<
            size_t         still_available;  //!< for bookeeping
            size_t         first_available;  //!< for bookeeping
            uint8_t       *data;
            const uint8_t *last;

            inline nugget(const size_t chunk_size,
                          void        *user_entry) throw() :
            provided_number( chunk_size / BLOCK_SIZE ),
            still_available( provided_number ),
            first_available( 0 ),
            data( static_cast<uint8_t *>(user_entry) ),
            last( data+chunk_size )
            {
                Y_STATIC_CHECK(BLOCK_SIZE>=sizeof(size_t),block_bits_too_small);
                assert(user_entry);
                std::cerr << "provided_number= " << provided_number << std::endl;
                // format
                uint8_t *p = data;
                for(size_t i=0; i != provided_number; p += BLOCK_SIZE )
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
                memset(p,0,BLOCK_SIZE);
                return p;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(nugget);
        };

    }
}

#endif
