#include "y/spade/ghost.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel {

            Ghost:: ~Ghost() throw()
            {
                memory::global::location().release_as(++indices,count_,bytes_);
                _bzset(items);
            }

            Ghost:: Ghost( size_t n ) :
            count_(n),
            bytes_(0),
            indices( memory::global::instance().acquire_as<size_t>(count_,bytes_)-1 ),
            items( n )
            {

            }

            size_t Ghost:: size() const throw()
            {
                return items;
            }

            const size_t & Ghost:: operator[](const size_t i) const throw()
            {
                assert(i>0);
                assert(i<=items);
                return indices[i];
            }

            
        }

        Ghost:: ~Ghost() throw()
        {
        }

        
        

    }

}

