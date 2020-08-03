#include "y/memory/small/piece.hpp"
#include <cstring>

namespace upsylon {

    namespace memory
    {

        namespace small
        {
            piece:: ~piece() throw()
            {
            }

            static inline
            uint8_t compute_blocks(const size_t block_size,
                                   const size_t chunk_size) throw()
            {
                assert(block_size>0);
                const size_t max_blocks = chunk_size/block_size;
                if(max_blocks>0xff)
                {
                    return 0xff;
                }
                else
                {
                    return static_cast<uint8_t>(max_blocks);
                }
            }

            piece:: piece(const size_t block_size,
                          void *       chunk_data,
                          const size_t chunk_size) throw() :
            next(0),
            prev(0),
            data( static_cast<uint8_t*>(chunk_data) ),
            last( data ),
            first_available(0),
            still_available(compute_blocks(block_size,chunk_size)),
            provided_number(still_available)
            {
                // setup
                last += provided_number*block_size;
                for(uint8_t q=0,*p=data; q!=provided_number; ++q)
                {
                    *(p++) = ++q;
                }

            }

            ownership piece:: owner_of(const void *addr) const throw()
            {
                const uint8_t *p = static_cast<const uint8_t*>(addr);
                if(p<data)
                {
                    return owned_by_prev;
                }
                else if(p>=last)
                {
                    return owned_by_next;
                }
                else
                {
                    assert(owns(addr));
                    return owned_by_this;
                }
            }

            bool piece:: owns(const void *addr)     const throw()
            {
                const uint8_t *p = static_cast<const uint8_t*>(addr);
                return (p>=data) && (p<last);
            }

            bool piece:: is_empty() const throw()
            {
                return still_available>=provided_number;
            }

            size_t piece::allocated() const throw()
            {
                assert(still_available<=provided_number);
                return provided_number-still_available;
            }


            void * piece::acquire(const size_t block_size) throw()
            {
                // sanity check
                assert(still_available>0);
                assert(still_available<=provided_number);

                // return object
                uint8_t *p = &data[ first_available*block_size ]; // get address
                first_available = *p;                             // read next available address
                --still_available;                                // bookeeping
                memset(p,0,block_size);                           // zero memory
                return p;                                         // done
            }

            bool piece:: is_aligned(const void *addr, const size_t block_size) const throw()
            {
                assert(addr!=NULL);
                assert(owns(addr));
                return (static_cast<ptrdiff_t>( static_cast<const uint8_t*>(addr)-data) % block_size) == 0;
            }

            void piece:: release(void *addr, const size_t block_size) throw()
            {
                assert(addr!=NULL);
                assert(owns(addr));
                assert(still_available<provided_number);
                assert(is_aligned(addr,block_size));

                // restore linking
                uint8_t *to_release = static_cast<uint8_t *>(addr);
                *to_release         = first_available;
                first_available     = static_cast<uint8_t>(to_release-data);
                assert( (to_release-data) == first_available );

                // bookkeeping
                ++still_available;
            }

        }
    }
}
