//! \file
#ifndef Y_MEMORY_ALLOCATOR_INCLUDED
#define Y_MEMORY_ALLOCATOR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace memory
    {
        //! allocator interface
        class allocator
        {
        public:
            virtual ~allocator() throw();

            const int64_t allocated;

            void *acquire( size_t &n );
            void  release( void * &p, size_t &n) throw();
            
        protected:
            explicit allocator() throw();
            virtual  void * __acquire(size_t &n) = 0;
            virtual  void   __release(void * &p, size_t &n) throw() = 0;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(allocator);
        };
    }
}

#endif

