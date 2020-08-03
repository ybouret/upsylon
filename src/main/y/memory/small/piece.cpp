#include "y/memory/small/piece.hpp"

namespace upsylon {

    namespace memory
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

            //uint8_t   *p = data;
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


    }
}
