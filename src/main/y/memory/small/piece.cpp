#include "y/memory/small/piece.hpp"

namespace upsylon {

    namespace memory
    {

        namespace small
        {
            //static const char fn[] = "[memory::small::piece]";

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

                for(uint8_t q=0,*p=data; q!=provided_number; p += block_size)
                {
                    assert(owns(p));
                    *p = ++q;
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

            bool piece:: is_aligned(const void *addr, const size_t block_size) const throw()
            {
                assert(addr!=NULL);
                assert(owns(addr));
                return (static_cast<ptrdiff_t>( static_cast<const uint8_t*>(addr)-data) % block_size) == 0;
            }

        }

    }

}

#include <cstring>
namespace upsylon {

    namespace memory
    {

        namespace small
        {

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



            void piece:: release(void *addr, const size_t block_size) throw()
            {
                assert(addr!=NULL);
                assert(owns(addr));
                assert(still_available<provided_number);
                assert(is_aligned(addr,block_size));

                // restore linking
                uint8_t *to_release = static_cast<uint8_t *>(addr);
                *to_release         = first_available;
                const size_t indx   = static_cast<size_t>(to_release-data)/block_size; assert(indx<size_t(provided_number));
                first_available     = static_cast<uint8_t>(indx);

                // bookkeeping
                ++still_available;
            }



        }
    }
}

#include "y/type/utils.hpp"

namespace upsylon {

    namespace memory
    {

        namespace small
        {

            size_t piece::min_chunk_size_for(const size_t block_size) throw()
            {
                return next_power_of_two(block_size);
            }

            
            size_t piece:: max_chunk_size_for(const size_t block_size) throw()
            {

                //--------------------------------------------------------------
                //
                // take the biggest required abd its boundary
                //
                //--------------------------------------------------------------

                size_t  blocks   = 0xff;
                size_t  required = blocks*block_size;
                size_t  boundary = next_power_of_two(required);

                //std::cerr << "boundary " << boundary << " for " << required << " #" << blocks << std::endl;

                //--------------------------------------------------------------
                //
                // build previous ratios
                //
                //--------------------------------------------------------------
                size_t prev_boundary = boundary >> 1;
                while( prev_boundary >1 )
                {
                    blocks                     = prev_boundary/block_size;
                    const size_t prev_required = blocks*block_size;
                    if(required*prev_boundary<prev_required*boundary)
                    {
                        required = prev_required;
                        boundary = prev_boundary;
                        //std::cerr << "better with " << boundary  << " for " << required << " #" << blocks << std::endl;
                        prev_boundary >>= 1;
                    }
                    else
                    {
                        break;
                    }

                }
                
                //--------------------------------------------------------------
                //
                // return winning boundary
                //
                //--------------------------------------------------------------
                return boundary;
            }

        }

    }

}
