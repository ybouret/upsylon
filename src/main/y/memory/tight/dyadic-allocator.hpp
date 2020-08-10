
//! \file

#ifndef Y_MEMORY_TIGHT_DYADIC_ALLOCATOR
#define Y_MEMORY_TIGHT_DYADIC_ALLOCATOR 1

#include "y/memory/allocator.hpp"
#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class objects; //!< forward declarartion
                      
            //__________________________________________________________________
            //
            //! allocator of 2^0...2^n blocks
            //__________________________________________________________________
            class dyadic_allocator : public memory::allocator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
               
                explicit dyadic_allocator(objects &) throw(); //!< setup using persistent objects
                virtual ~dyadic_allocator() throw();          //!< cleanup

                //______________________________________________________________
                //
                // allocator interface
                //______________________________________________________________
                virtual void *acquire(size_t &n);                     //!< acquire=>n=2^p
                virtual void  release(void * &p, size_t &n ) throw(); //!< release a previously allocated

            private:
                Y_DISABLE_COPY_AND_ASSIGN(dyadic_allocator);
                objects &objs;
            };

        }

    }
}


#endif
