
#include "y/memory/tight/stock.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace tight {

            stock:: ~stock() throw()
            {
                release();
            }

            stock:: stock( hoard &h ) :
            proxy(h),
            nodes()
            {
                if( h.block_size < min_block_size )
                {
                    throw libc::exception(EINVAL,"small::stock(block_size=%u<%u)", unsigned(h.block_size), unsigned(min_block_size) );
                }
            }

            size_t stock:: block_size() const throw()
            {
                return proxy.block_size;
            }

            size_t stock:: available() const throw()
            {
                return nodes.size;
            }

            void *stock:: query()
            {
                if( nodes.size )
                {
                    return nodes.query();
                }
                else
                {
                    return proxy.acquire();
                }
            }

            void stock:: store(void *addr) throw()
            {
                node_t *node = static_cast<node_t *>(addr);
                node->next   = 0;
                nodes.store(node);
            }


            void stock:: release() throw()
            {
                while(nodes.size)
                {
                    proxy.release(nodes.query());
                }
            }

            void stock:: reserve(size_t n)
            {
                while(n-- > 0 )
                {
                    store( proxy.acquire() );
                }
            }

        }

    }

}
