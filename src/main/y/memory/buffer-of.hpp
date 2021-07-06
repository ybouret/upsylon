//! \file

#ifndef Y_MEMORY_BUFFER_OF_INCLUDED
#define Y_MEMORY_BUFFER_OF_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include "y/sequence/addressable.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/buffer.hpp"

#include <cstring>

namespace upsylon
{
    namespace memory
    {


        //______________________________________________________________________
        //
        //! helper for buffer constructor
        //______________________________________________________________________
#define Y_MEMORY_BUFFER_OF_CTOR(N)        \
addressable<T>(), ro_buffer(),            \
items(N),                                 \
bytes(items*sizeof(T)),                   \
inMem(bytes),                             \
hmem(ALLOCATOR::instance()),              \
addr(hmem.acquire( aliasing::_(inMem) )), \
item(static_cast<mutable_type*>(addr)-1)

        //______________________________________________________________________
        //
        //
        //! buffer of integral types
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class buffer_of : public addressable<T>, public ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with size
            inline buffer_of(const size_t n) : Y_MEMORY_BUFFER_OF_CTOR(n) {}

            //! copy constructor
            inline buffer_of(const buffer_of &other) : collection(), Y_MEMORY_BUFFER_OF_CTOR(other.items)
            {
                memcpy(addr,other.addr,bytes);
            }

            //! copy an accessible array
            template <typename U>
            inline buffer_of( const accessible<U> &arr ) : Y_MEMORY_BUFFER_OF_CTOR( arr.size() )
            {
                for(size_t i=arr.size();i>0;--i)
                {
                    item[i] = arr[i];
                }
            }

            //! cleanup
            inline virtual ~buffer_of() throw()
            {
                hmem.release(*(void**)&addr,aliasing::_(inMem));
                aliasing::_(items)=0;
                aliasing::_(bytes)=0;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            inline size_t         allocated() const throw() { return inMem; } //!< info


            //__________________________________________________________________
            //
            // addressable interface
            //__________________________________________________________________
            inline virtual size_t size()      const throw() { return items; } //!< interface: addressable

            //! interface: addressable acces
            inline virtual type  &operator[](const size_t indx) throw()
            {
                assert(indx>0);
                assert(indx<=size());
                return item[indx];
            }

            //! interface: accessible acces
            inline virtual const_type & operator[](const size_t indx) const throw()
            {
                assert(indx>0);
                assert(indx<=size());
                return item[indx];
            }

            //__________________________________________________________________
            //
            // ro_buffer interface
            //__________________________________________________________________
            inline virtual const void * ro()     const throw() { return addr;  } //!< interface: buffer
            inline virtual size_t       length() const throw() { return bytes; } //!< interface: buffer


        private:
            const size_t  items;
            const size_t  bytes;
            const size_t  inMem;
            ALLOCATOR    &hmem;
            void         *addr;
            mutable_type *item;

            Y_DISABLE_ASSIGN(buffer_of);
        };

    }
}

#endif

