
//! \file

#ifndef Y_MEMORY_SMALL_QUARRY_ALLOCATOR_INCLUDED
#define Y_MEMORY_SMALL_QUARRY_ALLOCATOR_INCLUDED 1

#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class quarry; //!< forward declaration

            class quarry_allocator
            {
            public:
                explicit quarry_allocator(lockable &usr_access,
                                          quarry   &usr_quarry) throw();

                virtual ~quarry_allocator() throw();

                void *acquire(size_t &bytes, size_t &shift); //!< clean memory
                void  release(void *&addr, size_t &bytes, size_t &shift) throw();

                uint8_t *acquire_bytes(size_t  &bytes, size_t &shift);
                void     release_bytes(uint8_t *&addr, size_t &bytes, size_t &shift) throw();

                template <typename T>
                T *acquire_field(size_t &count,size_t &bytes, size_t &shift)
                {
                    try
                    {
                        if(count<=0) count=1;
                        void *p = acquire(bytes = count * sizeof(T),shift);
                        assert(bytes/sizeof(T)>=count);
                        count = bytes/sizeof(T);
                        return static_cast<T*>(p);
                    }
                    catch(...)
                    {
                        count=0;
                        bytes=0;
                        shift=0;
                        throw;
                    }
                }

                template <typename T>
                void release_field(T * &addr, size_t &count,size_t &bytes, size_t &shift) throw()
                {
                    assert(addr);
                    assert(count>0);
                    assert(bytes>0);
                    assert((1<<shift)==bytes);
                    release( *(void **)&addr, bytes, shift );
                    count = 0;
                    bytes = 0;
                    shift = 0;
                }

            private:
                lockable &Access;
                quarry   &Quarry;

                Y_DISABLE_COPY_AND_ASSIGN(quarry_allocator);
                void *fetch_locked(const size_t);
            };

        }

    }
}

#endif
