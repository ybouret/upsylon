//! \file
#ifndef Y_INK_PIXMAP_INCLUDED
#define Y_INK_PIXMAP_INCLUDED 1

#include "y/ink/bitmap.hpp"

namespace upsylon
{
    namespace ink
    {
        //! pixmap of any type
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            //! shadow row, mapping binary bitmap row
            class row
            {
            public:
                type        *p; //!< first iterm of the wrow
                const unit_t w; //!< row width
                //! item access
                inline type       & operator[](const unit_t x) throw()       { assert(x>=0);assert(x<w); assert(p); return p[x]; }
                //! item access, const
                inline const_type & operator[](const unit_t x) const throw() { assert(x>=0);assert(x<w); assert(p); return p[x]; }

            private:
                row(); ~row(); Y_DISABLE_COPY_AND_ASSIGN(row);
            };

            //! global memory pixmap
            inline explicit pixmap(const unit_t W, const unit_t H) :
            bitmap(sizeof(type),W,H)
            {
                assert(sizeof(type)==depth);
            }

            //! destructor
            inline virtual ~pixmap() throw()
            {
            }

            //! row access from bitmap rows
            inline row &       operator[](const unit_t y) throw()       { assert(y>=0); assert(y<sizes.y); return *(static_cast<row *>(rows)+y); }
            
            //! frow access from bitmap rows, const
            inline const row & operator[](const unit_t y) const throw() { assert(y>=0); assert(y<sizes.y); return *(static_cast<row *>(rows)+y); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(pixmap);
        };
    }
}

#endif
