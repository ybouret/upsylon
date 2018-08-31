//! \file
#ifndef Y_INK_PIXMAP_INCLUDED
#define Y_INK_PIXMAP_INCLUDED 1

#include "y/ink/bitmap.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! define metrics for zero flux computation
#define Y_PIXMAP_SIGNED_METRICS()  sw(w), ww( (sw+sw)-2 ), sh(h), hh( (sh+sh) - 2 )

        //! pixmap of any type
        template <typename T>
        class Pixmap : public Bitmap
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            const unit_t sw;     //!< signed width
            const unit_t ww;     //!< sw+sw
            const unit_t sh;     //!< signed height
            const unit_t hh;     //!< sh+sh

            //! shadow row, mapping binary bitmap row
            class Row
            {
            public:
                type        *p; //!< first iterm of the wrow
                const size_t w; //!< row width
                //! item access
                inline type       & operator[](const size_t x) throw()       { assert(x<w); assert(p); return p[x]; }
                //! item access, const
                inline const_type & operator[](const size_t x) const throw() { assert(x<w); assert(p); return p[x]; }

            private:
                Row(); ~Row(); Y_DISABLE_COPY_AND_ASSIGN(Row);
            };

            //! global memory pixmap
            inline explicit Pixmap(const size_t W, const size_t H) :
            Bitmap(sizeof(type),W,H), Y_PIXMAP_SIGNED_METRICS()
            {
                assert(sizeof(type)==depth);
            }

            //! shared pixmap with compatible size
            inline explicit Pixmap( Bitmap *bmp ) :
            Bitmap(bmp), Y_PIXMAP_SIGNED_METRICS()
            {
                if(depth!=sizeof(T)) throw exception("pixmap: incompatible depths!!!");
            }

            //! copy, relying on bitmap
            Pixmap(const Pixmap &other) :
            Bitmap(other), Y_PIXMAP_SIGNED_METRICS()
            {
            }

            //! destructor
            inline virtual ~Pixmap() throw()
            {
            }

            //! row access from bitmap rows
            inline Row &       operator[](const size_t y) throw()       { assert(y<h); return *(static_cast<Row *>(rows)+y); }
            
            //! frow access from bitmap rows, const
            inline const Row & operator[](const size_t y) const throw() { assert(y<h); return *(static_cast<Row *>(rows)+y); }

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
                    assert(y>=0&&y<sh);
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
            Y_DISABLE_ASSIGN(Pixmap);
        };
    }
}

#endif
