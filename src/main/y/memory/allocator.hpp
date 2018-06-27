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

            void  *acquire( size_t &n );
            void   release( void * &p, size_t &n) throw();
            
            //! just feed count
            template <typename T>
            T *acquire(size_t &count, size_t &bytes)
            {
                bytes = count * sizeof(T);
                try
                {
                    void *p = acquire(bytes);
                    assert(bytes>=count*sizeof(T));
                    count = bytes/sizeof(T);
                    return static_cast<T*>(p);
                }
                catch(...)
                {
                    assert(0==bytes);
                    count = 0;
                    throw;
                }
            }
            
            template <typename T>
            inline void release( T * &p, size_t &count, size_t &bytes) throw()
            {
                void *q = static_cast<void *>(p);
                release(p,bytes); assert(0==p); assert(0==bytes);
                count = 0;
                p     = 0;
            }
            
            
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

