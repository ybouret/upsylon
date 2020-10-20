//! \file
#ifndef Y_MEMORY_BUFFERS_INCLUDED
#define Y_MEMORY_BUFFERS_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon
{

    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! lightweight local buffer
        //
        //______________________________________________________________________
        class cblock : public rw_buffer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cblock(const size_t n); //!< setup with next_power_of_two
            virtual ~cblock() throw();       //!< cleanup

            //__________________________________________________________________
            //
            // rw_buffer interface
            //__________________________________________________________________
            virtual const void *ro()     const throw(); //!< block_addr
            virtual size_t      length() const throw(); //!< block_size
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cblock);
            const size_t block_size;
            const size_t block_exp2;
            void        *block_addr;
        };

        //______________________________________________________________________
        //
        //
        //! lightweight local typed buffer
        //
        //______________________________________________________________________
        template <typename T>
        class cppblock : public cblock, public addressable<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with rounding
            inline explicit cppblock(const size_t n) :
            cblock(n*sizeof(T)),
            items( length()/sizeof(T)),
            item(  ((mutable_type *)( ro() ))  - 1 )
            {
            }

            //! cleanup
            inline virtual ~cppblock() throw()
            {
                item = 0;
            }

            //__________________________________________________________________
            //
            // addressable interface
            //__________________________________________________________________

            //! items
            inline virtual size_t size() const throw() { return items; }

            //! in [1..size()]
            inline virtual const_type & operator[](const size_t indx) const throw()
            {
                assert(indx>=1);
                assert(indx<=size());
                return item[indx];
            }

            //! in [1..size()]
            inline virtual type & operator[](const size_t indx) throw()
            {
                assert(indx>=1);
                assert(indx<=size());
                return item[indx];
            }

            //__________________________________________________________________
            //
            // C-style access
            //__________________________________________________________________

            //! for access in [0..size()-1]
            inline const_type * operator*() const throw()
            {
                return (const_type *) ro();
            }

            //! for access in [0..size()-1]
            inline type * operator*()   throw()
            {
                return (type *) ro();
            }

        private:
            const size_t  items;
            mutable_type *item;
            Y_DISABLE_COPY_AND_ASSIGN(cppblock);
        };


    }

}

#endif
