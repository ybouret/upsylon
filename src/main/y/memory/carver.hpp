//! \file
#ifndef Y_MEMORY_CARVER_INCLUDED
#define Y_MEMORY_CARVER_INCLUDED 1

#include "y/memory/slice.hpp"
#include "y/memory/arena-of.hpp"

namespace upsylon {

    namespace memory
    {

        //! carve some memory slices
        class carver : public allocator
        {
        public:
            //! the default chunk size for memory allocation
            const size_t chunk_size;

            //! big enough and aligned for internal use
            static size_t compute_chunk_size(const size_t user_chunk_size) throw();

            //! initialize using chunk size
            explicit carver(const size_t user_chunk_size) throw();

            //! clean up
            virtual ~carver() throw();

            //! allocator interface
            virtual void *acquire(size_t &n);

            //! allocator interface
            virtual void  release(void * &p, size_t &n) throw();
            
        private:
            slice               *acquiring; //!< previously used
            core::list_of<slice> slices;    //!< live slices, ranked by increasing memory
            arena_of<slice>      blocks;    //!< memory for one slice layout
            
            Y_DISABLE_COPY_AND_ASSIGN(carver);


        };
    }

}


#endif

