
//! \file

#ifndef Y_MEMORY_TIGHT_QUARRY_ALLOCATOR_INCLUDED
#define Y_MEMORY_TIGHT_QUARRY_ALLOCATOR_INCLUDED 1

#include "y/lockable.hpp"
#include <iostream>
#include "y/type/spec.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

            class quarry; //!< forward declaration

            //__________________________________________________________________
            //
            //! allocator with bytes=2^shift markers
            //__________________________________________________________________
            class quarry_allocator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                
                //! set up with permanent user's data
                explicit quarry_allocator(lockable &usr_access,
                                          quarry   &usr_quarry) throw();

                //! cleanup
                virtual ~quarry_allocator() throw();

                //______________________________________________________________
                //
                // anonymous allocation
                //______________________________________________________________
                
                //! acquire next power of two bytes=2^shift, clean memory
                void *acquire(size_t &bytes, size_t &shift);
                
                //! release a previously allocated block
                void  release(void *&addr, size_t &bytes, size_t &shift) throw();

                //______________________________________________________________
                //
                // bytes allocation
                //______________________________________________________________
                //! acquire with void* -> uint8_t * conversion
                uint8_t *acquire_bytes(size_t  &bytes, size_t &shift);
               
                //! release previously allocated bytes
                void     release_bytes(uint8_t *&addr, size_t &bytes, size_t &shift) throw();

                //______________________________________________________________
                //
                // fields allocation
                //______________________________________________________________
                
                //! acquire (count * sizeof(T) <= bytes = 2^shift)/sizeof(T) objects
                template <typename T> inline
                T *acquire_field(size_t &count,size_t &bytes, size_t &shift)
                {
                    try
                    {
                        if(count<=0) count=1;
                        void *p = acquire( (bytes=count*sizeof(T)),shift);
                        assert(bytes/sizeof(T)>=count);
                        count = bytes/sizeof(T);
                        //std::cerr << count << "/" << bytes << "=2^" << shift << std::endl;
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

                //! release a previously acquired field
                template <typename T> inline
                void release_field(T * &addr, size_t &count,size_t &bytes, size_t &shift) throw()
                {
                    assert(addr);
                    assert(count>0);
                    assert(bytes>0);
                    assert((size_t(1)<<shift)==bytes);
                    release( *(void **)&addr, bytes, shift );
                    assert(0==addr);
                    assert(0==bytes);
                    assert(0==shift);
                    count = 0;
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
