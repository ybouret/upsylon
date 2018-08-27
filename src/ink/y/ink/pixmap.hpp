//! \file
#ifndef Y_INK_PIXMAP_INCLUDED
#define Y_INK_PIXMAP_INCLUDED 1

#include "y/ink/bitmap.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ink
    {
#define Y_PIXMAP_SIGNED_METRICS()  sw(w), ww(w+w), sh(h), hh(sh+sh)

        //! pixmap of any type
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            const unit_t sw;     //!< signed width
            const unit_t ww;     //!< sw+sw
            const unit_t sh;     //!< signed height
            const unit_t hh;     //!< sh+sh

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
            bitmap(sizeof(type),W,H), Y_PIXMAP_SIGNED_METRICS()
            {
                assert(sizeof(type)==depth);
            }

            //! shared pixmap with compatible size
            inline explicit pixmap( bitmap *bmp ) :
            bitmap(bmp), Y_PIXMAP_SIGNED_METRICS()
            {
                if(depth!=sizeof(T)) throw exception("pixmap: incompatible depths!!!");
            }

            //! copy, relying on bitmap
            pixmap(const pixmap &other) :
            bitmap(other), Y_PIXMAP_SIGNED_METRICS()
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

            //! zero flux y
            inline unit_t zfy(const unit_t y) const throw()
            {
                if(y<0)
                {
                    return zfy(-y);
                }
                else if(y>=sh)
                {
                    return zfy(hh-y);
                }
                else
                {
                    return y;
                }
            }

            //! zero flux x
            inline unit_t zfx(const unit_t x) const throw()
            {
                if(x<0)
                {
                    return zfx(-x);
                }
                else if(x>=sw)
                {
                    return zfx(ww-x);
                }
                else
                {
                    return x;
                }
            }



        private:
            Y_DISABLE_ASSIGN(pixmap);
        };
    }
}

#endif
