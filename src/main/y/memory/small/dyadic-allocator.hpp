
//! \file

#ifndef Y_MEMORY_SMALL_DYADIC_ALLOCATOR
#define Y_MEMORY_SMALL_DYADIC_ALLOCATOR 1

#include "y/memory/allocator.hpp"
#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class objects;

            class dyadic_allocator : public memory::allocator
            {
            public:
                explicit dyadic_allocator( objects &usr ) throw();
                virtual ~dyadic_allocator() throw();

                virtual void *acquire(size_t &n);
                virtual void  release(void * &p, size_t &n ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(dyadic_allocator);
                objects &objs;
            };

        }

    }
}


#endif
