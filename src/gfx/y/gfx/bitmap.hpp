
//! \file

#ifndef Y_GFX_BITMAP_INCLUDED
#define Y_GFX_BITMAP_INCLUDED 1

#include "y/gfx/area.hpp"
#include "y/gfx/bitmap/pixels.hpp"
#include "y/gfx/bitmap/pixrow.hpp"

namespace upsylon
{
    namespace GFX
    {

        //______________________________________________________________________
        //
        //
        //! binary bitmap
        //
        //______________________________________________________________________
        class Bitmap : public Object
        {
        public:
            typedef PixRow Row;


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            explicit Bitmap(const unit_t W,
                            const unit_t H,
                            const size_t BPP);
            //! cleanup
            virtual ~Bitmap() throw();

            //! sub-bitmap, shared pixels
            explicit Bitmap(const Bitmap &bmp, const Area &area);


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const Bitmap &);

            void ldz() throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t w;          //!< width
            const unit_t h;          //!< height
            const unit_t bpp;        //!< bytes per pixel
            const unit_t scanline;   //!< w*bpp
            const unit_t stride;     //!< >= scanline
            const Pixels pixels;     //!< shared memory
            PixRows      rows;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
        };

    }
}

#endif
