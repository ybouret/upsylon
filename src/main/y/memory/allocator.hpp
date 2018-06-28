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
            const int64_t allocated; //!< bookeeping of bytes

            //! desctructor
            virtual ~allocator() throw();

            //! proxy for __acquire
            void  *acquire( size_t &n );
            //! proxy for __release
            void   release( void * &p, size_t &n) throw();

            //! acquire>=count objects, bytes is computed
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

            //! release previously allocated objects
            template <typename T>
            inline void release( T * &p, size_t &count, size_t &bytes) throw()
            {
                void *q = static_cast<void *>(p);
                release(q,bytes); assert(0==q); assert(0==bytes);
                count = 0;
                p     = 0;
            }
            
            
        protected:
            //! constructor, set allocated=0
            explicit allocator() throw();

            //! virtual acquire for n>0
            virtual  void * __acquire(size_t &n) = 0;

            //! virtual release for n>0, p!=0
            virtual  void   __release(void * &p, size_t &n) throw() = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(allocator);
        };
    }
}

#endif

