//! \file

#ifndef Y_MEMORY_TIGHT_TWAIN_ALLOCATOR_INCLUDED
#define Y_MEMORY_TIGHT_TWAIN_ALLOCATOR_INCLUDED 1

#include "y/memory/tight/exp2-allocator.hpp"

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {

            class twain_allocator : public exp2_allocator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit twain_allocator(lockable    &usr_access,
                                         quarry      &usr_quarry,
                                         const size_t usr_chunk_size,
                                         const size_t usr_lower_size,
                                         const size_t usr_limit_size);

                virtual ~twain_allocator() throw();


                //______________________________________________________________
                //
                // exp2_allocator interface
                //______________________________________________________________

                //! acquire next power of two bytes=2^shift, clean memory
                virtual void *acquire(size_t &bytes, size_t &shift);

                //! release a previously allocated block
                virtual void  release(void *&addr, size_t &bytes, size_t &shift) throw();

                //! garbage collection
                virtual void gc() throw();

                //______________________________________________________________
                //
                // specific methods
                //______________________________________________________________

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t lower_size; //!< minimal allocated size
                const size_t lower_exp2; //!< lower_size = 1 << lower_exp2
                const size_t limit_size; //!< size under which an arena is used
                const size_t limit_exp2; //!< limit_size = 1 << limit_exp2
                const size_t num_arenas; //!< 1+limit_exp2



            private:
                Y_DISABLE_COPY_AND_ASSIGN(twain_allocator);
                void        *A;         //!< arenas entry point
                void        *Z;         //!< zcache<chunks> entry point
                char        *workspace; //!< internal memory for A+Z
                const size_t work_exp2; //!< internal memory = 1 << work_exp2

                void release_A(size_t) throw();
                void release_W() throw();
                void release_Z() throw();
            };

        }
    }
}


#endif
