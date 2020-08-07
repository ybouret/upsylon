//! \file
#ifndef Y_MEMORY_BUFFERS_INCLUDED
#define Y_MEMORY_BUFFERS_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/buffer.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{
    namespace memory
    {
        //! flexible local buffers
        template <typename T,typename ALLOCATOR>
        class buffer_of : public counted_object, public memory::rw_buffer
        {
        public:

            //! acquire memory, instance is automatically created
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
            inline T *       operator*(void) throw()       { return data; }

            //! data access const
            inline const T * operator*(void) const throw() { return data; }

            const size_t size; //!< workspace

            //! buffer interface
            inline virtual const void *ro()     const throw() { return data; }

            //! buffer interface
            inline virtual size_t      length() const throw() { return size*sizeof(T); }

            //! swap
            inline void swap_with( buffer_of &other ) throw()
            {
                _cswap(size,other.size);
                cswap(items,other.items);
                cswap(bytes,other.bytes);
                cswap(data,other.data);
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(buffer_of);
            
            size_t items;
            size_t bytes;
            T     *data;
        };

        //! global buffer
        template <typename T>
        class global_buffer_of : public buffer_of<T,memory::global>
        {
        public:
            typedef buffer_of<T,memory::global> buffer_type; //!< base class
            //! constructor
            inline explicit global_buffer_of(const size_t n) : buffer_type(n) {}
            //! destructor
            inline virtual ~global_buffer_of() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(global_buffer_of);
        };

        //! pooled buffer
        template <typename T>
        class pooled_buffer_of : public buffer_of<T,memory::pooled>
        {
        public:
            typedef buffer_of<T,memory::pooled> buffer_type; //!< base class
            //! constructor
            inline explicit pooled_buffer_of(const size_t n) : buffer_type(n) {}
            //! destructor
            inline virtual ~pooled_buffer_of() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pooled_buffer_of);
        };

    }
}

#endif

