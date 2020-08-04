
#include "y/memory/small/blocks.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            blocks:: ~blocks() throw()
            {
            }



            blocks:: blocks(const size_t usr_chunk_size,
                            const size_t usr_limit_size) :
            chunk_size( next_power_of_two( max_of(usr_chunk_size,usr_limit_size,min_chunk_size) ) ),
            slots_size(most_significant_bit_mask(chunk_size/sizeof(blocks::slot_type))),
            slots_mask(slots_size-1 ),
            limit_size(usr_limit_size)
            {
                
            }

        }

    }

}

