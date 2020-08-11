
//! \file

#ifndef Y_MEMORY_JOINT_WARD_ALLOCATOR_INCLUDED
#define Y_MEMORY_JOINT_WARD_ALLOCATOR_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            class ward;

            class ward_allocator : public memory::allocator
            {
            public:

                lockable &L;
                ward     &W;

                explicit ward_allocator( lockable &l, ward &w ) throw();
                virtual ~ward_allocator() throw();

                virtual void *acquire(size_t &n);
                virtual void  release(void * &p, size_t &n ) throw();
                bool          compact( void * &addr, size_t &capa, const size_t size) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ward_allocator);
            };
        }

    }
}
#endif

