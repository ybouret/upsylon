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
                S.reset();
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

                acquiring = releasing = empty_one = S.push_back( section_for(0) );

            }

             tight::vein & ward:: vein_for(const size_t required)
            {
                size_t       shift=0;
                const size_t bytes = section::bytes_to_hold(required,shift); assert(bytes<=tight::vein::max_size);

                if(bytes<=chunk_size)
                {
                    assert(bytes<=V.block_size);
                    assert(shift<=V.block_exp2);
                    return V;
                }
                else
                {
                    return Q[shift];
                }

            }

            section     * ward:: section_for(const size_t required)
            {
                tight::vein &v = vein_for(required);
                section     *s = Z.zquery();
                try {
                    return new (s) section( v.acquire(), v.block_size, v.block_exp2 );
                }
                catch(...)
                {
                    Z.zstore(s);
                    throw;
                }
            }


        }
    }

}
