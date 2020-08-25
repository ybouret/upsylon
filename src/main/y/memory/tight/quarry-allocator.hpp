
//! \file

#ifndef Y_MEMORY_TIGHT_QUARRY_ALLOCATOR_INCLUDED
#define Y_MEMORY_TIGHT_QUARRY_ALLOCATOR_INCLUDED 1

#include "y/memory/tight/exp2-allocator.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {


            //__________________________________________________________________
            //
            //
            //! allocator with bytes=2^shift markers
            //
            //__________________________________________________________________
            class quarry_allocator : public exp2_allocator
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
                // exp2_allocator interface
                //______________________________________________________________
                
                //! acquire next power of two bytes=2^shift, clean memory
                virtual void *acquire(size_t &bytes, size_t &shift);
                
                //! release a previously allocated block
                virtual void  release(void *&addr, size_t &bytes, size_t &shift) throw();


                virtual void   gc()                 throw(); //!< do nothing
                virtual size_t get_min_size() const throw(); //!< min_size=vein::min_size
                virtual size_t get_min_exp2() const throw(); //!< min_exp2=vein::min_exp2


            private:
                Y_DISABLE_COPY_AND_ASSIGN(quarry_allocator);
                void *fetch_locked(const size_t);
            };
            
            

        }

    }
}

#endif
