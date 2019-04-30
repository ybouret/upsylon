//! \file
#ifndef Y_MEMORY_CARVER_INCLUDED
#define Y_MEMORY_CARVER_INCLUDED 1

#include "y/memory/slice.hpp"
#include "y/memory/allocator.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace memory
    {

        //! carve some memory slices
        class carver : public allocator
        {
        public:
            //! big enough and aligned for internal use
            static size_t compute_chunk_size(const size_t user_chunk_size) throw();

            const size_t  chunk_size;                                   //!< the default chunk size for memory allocation
            explicit      carver(const size_t user_chunk_size) throw(); //!< initialize using chunk size
            virtual      ~carver() throw();                             //!< clean up
            virtual void *acquire(size_t &n);                           //!< allocator interface: acquire
            virtual void  release(void * &p, size_t &n) throw();        //!< allocator interface: release

            //! try to compact memory
            bool          compact( void * &addr, size_t &capa, const size_t size ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(carver);
            void clr() throw();

            slice               *acquiring; //!< previously used
            core::list_of<slice> slices;    //!< live slices, ranked by increasing memory
            void *               wksp[19];  //!< ugly hack
            void                *impl;      //!< where the arena_of<slice> is
        };
    }

}


#endif

