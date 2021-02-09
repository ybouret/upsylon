
//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"

namespace upsylon
{
    namespace GFX
    {

        template <typename T>
        class Pixmap : public Bitmap
        {
        public:
            class Row
            {
            private:
                Row(); ~Row();
                Y_DISABLE_COPY_AND_ASSIGN(Row);
                T *p;
            public:
                const unit_t w;

                inline T & operator[](unit_t i) throw()
                {
                    assert(p); assert(i>=0); assert(i<w); return p[i];
                }

                inline const T & operator[](unit_t i) const throw()
                {
                    assert(p); assert(i>=0); assert(i<w); return p[i];
                }

            };

            explicit Pixmap(const unit_t W, const unit_t H) :
            Bitmap(W,H,sizeof(T)),
            _row( static_cast<Row *>(oor_rows()) )
            {
            }

            virtual ~Pixmap() throw()
            {
            }

            inline Row & operator[](const unit_t j) throw()
            {
                assert(j>=0);
                assert(j<h);
                return _row[j];
            }

            inline const Row & operator[](const unit_t j) const throw()
            {
                assert(j>=0);
                assert(j<h);
                return _row[j];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
            Row *_row;
        };

    }

}

#endif
