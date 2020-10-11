
#include "y/memory/tight/supply.hpp"
#include "y/object.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {
            supply::supply(const size_t block_size) :
            xnodes(upsylon::object::proto(), max_of(block_size,sizeof(node_type)) ),
            znodes(),
            block_size( xnodes.block_size() )
            {
            }

            supply:: ~supply() throw()
            {
                prune();
            }



            void supply:: fetch(size_t n)
            {
                Y_LOCK(xnodes.access);
                while(n-- > 0)
                {
                    znodes.store( static_cast<node_type*>(xnodes.acquire()) );
                }
            }

            void supply:: limit(const size_t n)
            {
                Y_LOCK(xnodes.access);
                while(znodes.size>n)
                {
                    xnodes.release_unlocked( znodes.query() );
                }
            }

            void supply:: prune() throw()
            {
                limit(0);
            }

            size_t supply:: prefetched() const throw()
            {
                return znodes.size;
            }

            
            void * supply:: query_block()
            {
                if(znodes.size>0)
                {
                    void *blk = znodes.query();
                    memset(blk,0,block_size);
                    return  (blk);
                }
                else
                {
                    return  (xnodes.acquire());
                }
            }

            void  supply:: store_block(void *args) throw()
            {
                assert(args);
                memset(args,0,sizeof(node_type));
                znodes.store( static_cast<node_type *>(  (args) ) );
            }

        }

    }

}


