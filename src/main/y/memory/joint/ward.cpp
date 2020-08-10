#include "y/memory/joint/ward.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            static inline
            size_t ward_chunk_size(const size_t chunk_size)
            {
                return next_power_of_two( clamp(tight::vein::min_size,chunk_size,tight::vein::max_size) );
            }

            ward:: ward( const size_t usr_chunk_size ) :
            chunk_size( ward_chunk_size(usr_chunk_size) ),
            Q(),
            Z(chunk_size,Q)
            {

            }

        }
    }

}
