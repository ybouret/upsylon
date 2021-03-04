
//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"
#include "y/gfx/pixrow.hpp"

namespace upsylon
{
    namespace graphic
    {

        template <typename T>
        class pixmap : public bitmap
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef pixrow<T> row;

            inline explicit pixmap(const unit_t W, const unit_t H) :
            bitmap(W,H,sizeof(T)),
            rows(0)
            {
                setup();
            }

            inline virtual ~pixmap() throw()
            {
            }

            inline pixmap(const pixmap &other) throw() :
            bitmap(other),
            rows(0)
            {
                setup();
            }

            inline row & operator()(const unit_t j) throw()
            {
                assert(j>=0);assert(j<h);
                return rows[j];
            }

            inline const row & operator()(const unit_t j) const throw()
            {
                assert(j>=0);assert(j<h);
                return rows[j];
            }

            inline row & operator[](const unit_t j) throw()
            {
                return rows[ zfh(j) ];
            }

            inline const row & operator[](const unit_t j) const throw()
            {
                return rows[ zfh(j) ];
            }


        private:
            row *rows;

            Y_DISABLE_ASSIGN(pixmap);
            inline void setup() throw() { rows = static_cast<row *>(a_rows->impl.block_addr); }


        };
    }

}

#endif

