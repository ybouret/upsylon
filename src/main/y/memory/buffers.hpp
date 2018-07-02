//! \file
#ifndef Y_MEMORY_BUFFERS_INCLUDED
#define Y_MEMORY_BUFFERS_INCLUDED 1

#include "y/memory/pooled.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace memory
    {
        //! flexible local buffers
        template <typename T,typename ALLOCATOR>
        class buffer_of : public memory::rw_buffer
        {
        public:
            //! acquire memory
            inline explicit buffer_of(const size_t n) :
            size(n),
            items(size),
            bytes(0),
            data( ALLOCATOR::instance().template acquire_as<T>(items,bytes) )
            {}

            //! release memory
            inline  virtual ~buffer_of() throw()
            {
                ALLOCATOR::location().template release_as<T>(data,items,bytes);
            }

            //! data access
            inline T *       operator()(void) throw()       { return data; }

            //! data access const
            inline const T * operator()(void) const throw() { return data; }

            const size_t size; //!< workspace

            //! buffer interface
            inline virtual const void *ro()     const throw() { return data; }

            //! buffer interface
            inline virtual size_t      length() const throw() { return size*sizeof(T); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(buffer_of);
            
            size_t items;
            size_t bytes;
            T     *data;
        };
    }
}

#endif

