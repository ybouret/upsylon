
#include "y/memory/tight/hoard.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            hoard:: ~hoard() throw() { _bzset(block_size); }

            hoard:: hoard( lockable &l, const size_t bs ) throw() :
            access(l), block_size(bs)
            {
                assert(bs>0);
            }


            void * hoard:: acquire()
            {
                Y_LOCK(access);
                return on_acquire();
            }

            void hoard:: release(void *addr) throw()
            {
                Y_LOCK(access);
                assert(addr);
                on_release(addr);
            }
        }

    }

}

