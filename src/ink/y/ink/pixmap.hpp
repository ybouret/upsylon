//! \file

#ifndef Y_INK_PIXMAP_INCLUDED
#define Y_INK_PIXMAP_INCLUDED 1

#include "y/ink/bitmap.hpp"

namespace upsylon
{
    namespace ink
    {
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            Y_DECL_ARGS(T,type);
            class row
            {
            public:
                type        *p;
                const unit_t w;
                inline type       & operator[](const unit_t x) throw()       { assert(x>=0);assert(x<w); assert(p); return p[x]; }
                inline const_type & operator[](const unit_t x) const throw() { assert(x>=0);assert(x<w); assert(p); return p[x]; }

            private:
                row(); ~row(); Y_DISABLE_COPY_AND_ASSIGN(row);
            };

            inline explicit pixmap(const unit_t W, const unit_t H) :
            bitmap(W,H,sizeof(type))
            {
            }

            inline virtual ~pixmap() throw()
            {
            }

            inline row &       operator[](const unit_t y) throw()       { assert(y>=0); assert(y<sizes.y); return *(static_cast<row *>(rows)+y); }
            inline const row & operator[](const unit_t y) const throw() { assert(y>=0); assert(y<sizes.y); return *(static_cast<row *>(rows)+y); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(pixmap);
        };
    }
}

#endif
