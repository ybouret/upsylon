
//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! named Pixmap
        //
        //______________________________________________________________________
        template <typename T>
        class Pixmap : public Bitmap
        {
        public:
            //! ABI of PixRow
            class Row
            {
            private:
                Row(); ~Row();
                Y_DISABLE_COPY_AND_ASSIGN(Row);
                T *p;
            public:
                const unit_t    w;
                const ZeroFlux &z;

                //! Zero Flux pixel[i], const
                inline T & operator()(const unit_t i)             throw() { assert(p); return p[z(i)]; }

                //! Zero Flux pixel[i]
                inline const T & operator()(const unit_t i) const throw() { assert(p); return p[z(i)]; }

                //! Raw pixel[i]
                inline T & operator[](const unit_t i) throw()
                {
                    assert(p); assert(i>=0); assert(i<w); return p[i];
                }

                //! Raw pixel[i], const
                inline const T & operator[](const unit_t i) const throw()
                {
                    assert(p); assert(i>=0); assert(i<w); return p[i];
                }


            };

            inline explicit Pixmap(const unit_t W, const unit_t H) :
            Bitmap(W,H,sizeof(T)),
            _row( static_cast<Row *>(oor_rows()) )
            {
            }

            inline virtual ~Pixmap() throw()
            {
            }

            //! Zero Flux Row[j]
            inline Row & operator()(const unit_t j) throw()
            {
                return _row[ rows.zfh(j) ];
            }

            //! Zero Flux Row[j], const
            inline const Row & operator()(const unit_t j) const throw()
            {
                return _row[ rows.zfh(j) ];
            }

            //! Raw Row[j]
            inline Row & operator[](const unit_t j) throw()
            {
                assert(j>=0); assert(j<h);
                return _row[j];
            }

            //! Raw Row[j], const
            inline const Row & operator[](const unit_t j) const throw()
            {
                assert(j>=0); assert(j<h);
                return _row[j];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
            Row *_row;
        };

    }

}

#endif
