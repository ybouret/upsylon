
//! \file

#ifndef Y_MEMORY_TIGHT_EXP2_ALLOCATOR_INCLUDED
#define Y_MEMORY_TIGHT_EXP2_ALLOCATOR_INCLUDED 1

#include "y/lockable.hpp"

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {
            class quarry; //! forward declaration

            class exp2_allocator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~exp2_allocator() throw();

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________

                //! acquire next power of two bytes=2^shift, clean memory
                virtual void *acquire(size_t &bytes, size_t &shift) = 0;

                //! release a previously allocated block
                virtual void  release(void *&addr, size_t &bytes, size_t &shift) throw() = 0;

                //! optional garbace collection
                virtual void gc() throw() = 0;

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                void optimize() throw();                  //!< locked optimize
                const quarry & operator*() const throw(); //!< access internal quarry R/O

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
                T *acquire_field(size_t &count, size_t &bytes, size_t &shift)
                {
                    try
                    {
                        if(count<=0) count=1;
                        void *p = acquire( (bytes=count*sizeof(T)),shift);
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

            protected:
                explicit exp2_allocator(lockable &, quarry &) throw();

                lockable &L; //!< shared access
                quarry   &Q; //!< shared quarry

            private:
                Y_DISABLE_COPY_AND_ASSIGN(exp2_allocator);
            };
        }
    }
}

#endif
