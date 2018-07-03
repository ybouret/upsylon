//! \file
#ifndef Y_MEMORY_SLAB_INCLUDED
#define Y_MEMORY_SLAB_INCLUDED 1

#include "y/memory/chunk.hpp"
#include "y/os/static-check.hpp"
#include "y/dynamic.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {
        //! memory I/O for a fixed number of objects
        template <typename T>
        class slab : public dynamic
        {
        public:
            static const size_t block_size   = Y_ALIGN_FOR_ITEM(size_t,sizeof(T));

            typedef __chunk<size_t> chunk_type;

            static inline
            size_t bytes_for( const size_t num_objects ) throw()
            {
                Y_STATIC_CHECK(block_size>=sizeof(T),slab_alignment);
                return block_size * num_objects;
            }

            inline slab(void        *chunk_data,
                        const size_t chunk_size) throw() :
            chunk(block_size,chunk_data,chunk_size)
            {

            }

            inline virtual ~slab() throw()
            {
                if(!chunk.is_empty())
                {
                    std::cerr << "[memory.slab] still #allocated=" << chunk.allocated() << std::endl;
                }
            }

            //! number of available memory object
            inline virtual size_t size() const throw()
            {
                return chunk.still_available;
            }

            //! orignal number of memory objects
            inline virtual size_t capacity() const throw()
            {
                return chunk.provided_number;
            }

            //! prepare memory area for sizeof(T)
            inline T *acquire() throw()
            {
                assert( size() > 0 );
                return static_cast<T*>(chunk.acquire());
            }

            inline void release(T *addr) throw()
            {
                chunk.release(static_cast<void*>(addr));
            }


        private:
            chunk_type chunk;
            Y_DISABLE_COPY_AND_ASSIGN(slab);
        };
    }
}

#endif

