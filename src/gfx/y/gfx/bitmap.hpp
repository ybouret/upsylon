
//! \file

#ifndef Y_GFX_BITMAP_INCLUDED
#define Y_GFX_BITMAP_INCLUDED 1

#include "y/gfx/area.hpp"
#include "y/gfx/bitmap/pixels.hpp"
#include "y/gfx/bitmap/pixrow.hpp"

namespace upsylon
{
    namespace hashing
    {
        class function; //!< forward declaration
    }

    namespace GFX
    {
        namespace Async
        {
            class Broker; //!< forward declaration
        }



        //______________________________________________________________________
        //
        //
        //! binary bitmap
        //
        //______________________________________________________________________
        class Bitmap : public Object, public Area
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            explicit Bitmap(const unit_t W,
                            const unit_t H,
                            const size_t D);
            //! cleanup
            virtual ~Bitmap() throw();

            //! sub-bitmap, shared pixels
            explicit Bitmap(const Bitmap &bmp, const Area &area);

            //! hard copy
            explicit Bitmap(const Bitmap  &bmp,
                            Async::Broker &broker);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const Bitmap &);

            //! memory to 0
            void ldz() throw();

            void *       addressOf(const Point p)       throw(); //!< agnostic address of
            const void * addressOf(const Point p) const throw(); //!< agnostic address of
            void         run(hashing::function&)  const throw(); //!< run on data
            hashing::function & hashWith(hashing::function&) const throw(); //!< set/run

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t   depth;      //!< bytes per pixel
            const unit_t   scanline;   //!< w*bpp
            const unit_t   stride;     //!< >= scanline
            const Pixels   pixels;     //!< shared memory
            PixRows        rows;       //!< precomputed rows

        protected:
            void *oor_rows() throw(); //!< out of reach first rows

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
        };

    }
}

#endif
