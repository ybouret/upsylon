//! \file
#ifndef Y_MEMORY_CARVER_INCLUDED
#define Y_MEMORY_CARVER_INCLUDED 1

#include "y/memory/slice.hpp"

namespace upsylon {

    namespace memory
    {

        class carver
        {
        public:
            const size_t chunk_size;

            static size_t compute_chunk_size(const size_t user_chunk_size) throw();

            explicit carver(const size_t user_chunk_size) throw();
            virtual ~carver() throw();

            void *acquire(size_t &n);

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

        public:
            const size_t bytes; //!< total bytes
            const size_t slices_per_page; //!< number of allocatable slices per page

        };
    }

}


#endif

