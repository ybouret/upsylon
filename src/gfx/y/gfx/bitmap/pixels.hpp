
//! \file

#ifndef Y_GFX_BITMAP_PIXELS_INCLUDED
#define Y_GFX_BITMAP_PIXELS_INCLUDED 1

#include "y/gfx/object.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! flat bytes for bitmaps
        //
        //______________________________________________________________________
        class Pixels_ : public Object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Pixels_(const size_t n); //!< setup
            virtual ~Pixels_() throw();       //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   bytes; //!< available bytes
            void *   const entry; //!< first available bytes

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const Pixels_ &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixels_);
        };

        //______________________________________________________________________
        //
        //
        //! shared pixels
        //
        //______________________________________________________________________
        typedef arc_ptr<const Pixels_> Pixels;

    }
}

#endif
