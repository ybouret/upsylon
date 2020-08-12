
#include "y/memory/tight/xcache.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

            xcache:: ~xcache() throw()
            {
            }

            xcache:: xcache(objects     &objs,
                            const size_t block_size) :
            access( objs.Access ),
            shared( objs.Blocks[block_size] )
            {
            }

            size_t xcache:: block_size() const throw()
            {
                return shared.block_size;
            }


            void *xcache:: acquire()
            {
                Y_LOCK(access);
                return shared.acquire();
            }

            void  xcache:: release(void *addr) throw()
            {
                Y_LOCK(access);
                shared.release(addr);
            }

            void * xcache:: acquire_unlocked()
            {
                return shared.acquire();
            }

            void  xcache:: release_unlocked(void *addr) throw()
            {
                shared.release(addr);
            }

        }

    }

}
