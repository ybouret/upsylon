
#include "y/memory/small/hoard.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            hoard:: ~hoard() throw() {}

            hoard:: hoard( lockable &l, const size_t bs ) throw() :
            access(l),
            block_size(bs)
            {
                assert(bs>0);
            }


            void * hoard:: query()
            {
                Y_LOCK(access);
                return on_query();
            }

            void hoard:: store(void *addr) throw()
            {
                Y_LOCK(access);
                assert(addr);
                on_store(addr);
            }
        }

    }

}

