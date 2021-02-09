
//! \file

#ifndef Y_GFX_BITMAP_INCLUDED
#define Y_GFX_BITMAP_INCLUDED 1

#include "y/gfx/types.hpp"
#include "y/gfx/bitmap/pixels.hpp"

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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const Bitmap &);

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


        protected:



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
        };

    }
}

#endif
