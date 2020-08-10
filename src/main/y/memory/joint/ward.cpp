#include "y/memory/joint/ward.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/exception.hpp"

#include <iostream>

namespace upsylon {

    namespace memory {

        namespace joint {

            static inline
            size_t ward_chunk_size(const size_t chunk_size)
            {
                return next_power_of_two( clamp(tight::vein::min_size,chunk_size,tight::vein::max_size) );
            }

            ward:: ~ward() throw()
            {
            }
            

            ward:: ward(const size_t usr_chunk_size) :
            acquiring(0),
            releasing(0),
            empty_one(0),
            S(),
            Q(),
            chunk_size( ward_chunk_size(usr_chunk_size) ),
            V(Q(chunk_size)),
            Z(chunk_size,Q)
            {
                std::cerr << "W.chunk_size=" << chunk_size   << std::endl;
                std::cerr << "Z.chunk_size=" << Z.chunk_size << std::endl;
                std::cerr << "|_nodes_rise=" << Z.nodes_rise << " section/part" << std::endl;
                
            }

#if 0
            tight::vein & ward:: vein_for(const size_t required)
            {
                const size_t bytes = section::bytes_to_hold(required);
                if(bytes<=chunk_size)
                {
                    return V;
                }
                else if(bytes>tight::vein::max_size)
                {
                    throw exception("too many bytes for ward");
                }
                else
                {
                    assert(bytes>chunk_size);
                    assert(bytes>=tight::vein::min_size);
                    return Q( next_power_of_two(bytes) );
                }

            }
#endif


        }
    }

}
