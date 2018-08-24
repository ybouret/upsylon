//! \file
#ifndef Y_INK_PIXMAP_INCLUDED
#define Y_INK_PIXMAP_INCLUDED 1

#include "y/ink/bitmap.hpp"
#include "y/exception.hpp"

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
                const size_t w; //!< row width
                //! item access
                inline type       & operator[](const size_t x) throw()       { assert(x<w); assert(p); return p[x]; }
                //! item access, const
                inline const_type & operator[](const size_t x) const throw() { assert(x<w); assert(p); return p[x]; }

            private:
                row(); ~row(); Y_DISABLE_COPY_AND_ASSIGN(row);
            };

            //! global memory pixmap
            inline explicit pixmap(const size_t W, const size_t H) :
            bitmap(sizeof(type),W,H)
            {
                assert(sizeof(type)==depth);
            }

            //! shared pixmap with compatible size
            inline explicit pixmap( bitmap *bmp ) :
            bitmap(bmp)
            {
                if(depth!=sizeof(T)) throw exception("pixmap: incompatible depths!!!");
            }

            //! copy, relying on bitmap
            pixmap(const pixmap &other) :
            bitmap(other)
            {
            }

            //! destructor
            inline virtual ~pixmap() throw()
            {
            }

            //! row access from bitmap rows
            inline row &       operator[](const size_t y) throw()       { assert(y<h); return *(static_cast<row *>(rows)+y); }
            
            //! frow access from bitmap rows, const
            inline const row & operator[](const size_t y) const throw() { assert(y<h); return *(static_cast<row *>(rows)+y); }


        private:
            Y_DISABLE_ASSIGN(pixmap);
        };
    }
}

#endif
