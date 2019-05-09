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
            //! desctructor
            virtual ~allocator() throw();

            //! proxy for __acquire
            virtual void  *acquire( size_t &n ) = 0;

            //! proxy for __release
            virtual void   release( void * &p, size_t &n) throw() = 0;

            //! acquire>=count objects, bytes is computed
            template <typename T>
            T *acquire_as(size_t &count, size_t &bytes)
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
            inline void release_as( T * &p, size_t &count, size_t &bytes) throw()
            {
                void *q = static_cast<void *>(p);
                release(q,bytes); assert(0==q); assert(0==bytes);
                count = 0;
                p     = 0;
            }

            uint8_t *acquire_bytes(size_t &n);                        //!< wrapper for byte-wise allocation
            void     release_bytes(uint8_t * &p, size_t &n ) throw(); //!< wrapper for byte-wise release
            
        protected:
            //! constructor, set allocated=0
            explicit allocator() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(allocator);
        };
    }
}

#endif

