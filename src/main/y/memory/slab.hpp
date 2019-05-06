//! \file
#ifndef Y_MEMORY_SLAB_INCLUDED
#define Y_MEMORY_SLAB_INCLUDED 1

#include "y/dynamic.hpp"
#include "y/memory/chunk.hpp"
#include "y/os/static-check.hpp"

namespace upsylon
{
    namespace memory
    {

        //! slab base class
        class slab : public dynamic
        {
        public:
            typedef __chunk<size_t> chunk_type; //!< internal data
            virtual ~slab() throw();            //!< cleanup


            //! number of available memory object
            virtual size_t size() const throw();

            //! orignal number of memory objects
            virtual size_t capacity() const throw();


        protected:
            explicit slab(const size_t block_size,          //|
                          void        *chunk_data,          //|
                          const size_t chunk_size) throw(); //!< setup
            chunk_type chunk;                               //!< handling data
        private:
            Y_DISABLE_COPY_AND_ASSIGN(slab);
        };

        //! memory I/O for a fixed number of objects
        template <typename T>
        class slab_of : public slab
        {
        public:
            static const size_t     block_size   = Y_ALIGN_FOR_ITEM(size_t,sizeof(T)); //!< aligned block size

            //! compute bytes to provide to hold num_objects
            static inline size_t bytes_for( const size_t num_objects ) throw()
            {
                Y_STATIC_CHECK(block_size>=sizeof(T),bad_block_size);
                return block_size * num_objects;
            }

            //! format unerlying chunk
            inline slab_of(void *chunk_data,const size_t chunk_size) throw() :
            slab(block_size,chunk_data,chunk_size) {}

            //! destructor
            inline virtual ~slab_of() throw() {}

            //! prepare memory area for sizeof(T)
            inline T *acquire() throw()
            {
                assert( size() > 0 );
                return static_cast<T*>(chunk.acquire());
            }

            //! release a previously allocated block
            inline void release(T *addr) throw()
            {
                chunk.release(static_cast<void*>(addr));
            }

            //! check memory
            inline bool owns(const T *addr) const throw()
            {
                return chunk.owns(addr);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(slab_of);
        };
    }
}

#endif

