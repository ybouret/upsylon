

//! \file

#ifndef Y_GFX_BITMAP_LINES_INCLUDED
#define Y_GFX_BITMAP_LINES_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! anonymous row of pixels
        //
        //______________________________________________________________________
        class PixRow
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            PixRow(void           *entry,
                   const unit_t    width,
                   const ZeroFlux &zflux) throw(); //!< setup
            ~PixRow() throw();                               //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void       * operator()(const unit_t i, const unit_t depth)       throw(); //!< shifted address
            const void * operator()(const unit_t i, const unit_t depth) const throw(); //!< shifted address

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            void *       const p; //!< first item
            const unit_t       w; //!< width
            const ZeroFlux    &z; //!< zero flux

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixRow);
        };

        //______________________________________________________________________
        //
        //
        //! smart pointers of rows
        //
        //______________________________________________________________________
        class PixRows
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            PixRows(const unit_t width,
                    const unit_t height,
                    const void  *entry,
                    const unit_t stride);
            //! cleanup
            ~PixRows() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            PixRow       & operator[](const unit_t j) throw();        //!< raw row
            const PixRow & operator[](const unit_t j) const throw();  //!< raw row

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t    h;
            const ZeroFlux  zfh;
            const ZeroFlux  zfw;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixRows);
            size_t  count;
            size_t  bytes;
            PixRow *row;

        };

    }

}

#endif
