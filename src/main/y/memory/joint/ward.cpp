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
                assert(section::min_size<=tight::vein::max_size);
                return next_power_of_two( clamp(section::min_size,chunk_size,tight::vein::max_size) );
            }

            ward:: ~ward() throw()
            {
                S.reset();
            }
            
            size_t ward:: chunk_size() const throw()
            {
                return V.block_size;
            }

            const ward::quarry_type & ward:: _quarry()   const throw()
            {
                return Q;
            }

            const ward::sections    & ward:: _sections()   const throw()
            {
                return S;
            }

            

            ward:: ward(const size_t usr_chunk_size) :
            acquiring(0),
            S(),
            Q(),
            V(Q(ward_chunk_size(usr_chunk_size))),
            Z(V.block_size,Q)
            {
                std::cerr << "W.chunk_size=" << chunk_size()   << std::endl;
                std::cerr << "Z.chunk_size=" << Z.chunk_size << std::endl;
                std::cerr << "|_nodes_rise=" << Z.nodes_rise << " section/part" << std::endl;

                acquiring = S.push_back( section_for(0) );

            }

            tight::vein & ward:: vein_for(const size_t required)
            {
                size_t       shift=0;
                const size_t bytes = section::bytes_to_hold(required,shift); assert(bytes<=tight::vein::max_size);

                if(bytes<=V.block_size)
                {
                    assert(bytes<=V.block_size);
                    assert(shift<=V.block_exp2);
                    return V;
                }
                else
                {
                    assert(shift>=tight::vein::min_exp2);
                    assert(shift<=tight::vein::max_exp2);
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
