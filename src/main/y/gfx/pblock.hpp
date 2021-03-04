
//! \file

#ifndef Y_GRAPHIC_PBLOCK_INCLUDED
#define Y_GRAPHIC_PBLOCK_INCLUDED 1

#include "y/gfx/pixels.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! block/pixels
        //
        //______________________________________________________________________
        template <typename T>
        class pblock : public pixels
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases

            //! allocate for n items
            inline explicit pblock(const size_t n) :
            pixels( n * sizeof(T) ),
            entry( static_cast<mutable_type *>(block_addr) ),
            size(n),
            maxi(block_size/sizeof(T))
            {
            }

            //! cleanup
            inline virtual ~pblock() throw()
            {
                entry = NULL;
            }

            //! access [0..size-1]
            type & operator[](const size_t i) throw()
            {
                assert(i<size);
                return entry[i];
            }

            //! access [0..size-1]
            const_type & operator[](const size_t i) const throw()
            {
                assert(i<size);
                return entry[i];
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(pblock);
            mutable_type *entry; //!< first item address
        public:
            const size_t  size;  //!< designed items
            const size_t  maxi;  //!< allocated items
        };
    }

}

#endif
