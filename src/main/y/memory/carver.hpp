//! \file
#ifndef Y_MEMORY_CARVER_INCLUDED
#define Y_MEMORY_CARVER_INCLUDED 1

#include "y/memory/slice.hpp"

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
            //! memory for slices mapping
            struct page
            {
                page *next;
            };

            slice               *acquiring; //!< cache
            core::list_of<slice> slices;    //! live slices
            core::pool_of<slice> cached;    //! pool of dead slices
            core::pool_of<page>  pages;     //! ever growing memory to store slices

            Y_DISABLE_COPY_AND_ASSIGN(carver);
            
        public:
            const size_t bytes;           //!< total bytes (pages+slices)
            const size_t slices_per_page; //!< number of allocatable slices per page

        };
    }

}


#endif

