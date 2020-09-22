
#include "y/memory/tight/supply.hpp"
#include "y/object.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {
            supply::supply(const size_t block_size) :
            xnodes(upsylon::object::proto(),
                   max_of(block_size,sizeof(node_type))
                   ),
            znodes()
            {
            }

            supply:: ~supply() throw()
            {
                prune();
            }

            void supply:: prune() throw()
            {
                Y_LOCK(xnodes.access);
                while(znodes.size>0)
                {
                    xnodes.release_unlocked( znodes.query() );
                }
            }

            void supply:: fetch(size_t n)
            {
                Y_LOCK(xnodes.access);
                while(n-- > 0)
                {
                    znodes.store( static_cast<node_type*>(xnodes.acquire()) );
                }
            }

            size_t supply:: block_size() const throw()
            {
                return xnodes.block_size();
            }

            size_t supply:: node_bytes() const throw()
            {
                return sizeof(node_type);
            }


            size_t supply:: prefetched() const throw()
            {
                return znodes.size;
            }

            void * supply:: query_block()
            {
                return ( znodes.size>0 ) ? znodes.query() : xnodes.acquire();
            }

            void  supply:: store_block(void *args) throw()
            {
                assert(args);
                memset(args,0,sizeof(node_type));
                znodes.store( static_cast<node_type *>(args) );
            }

        }

    }

}


