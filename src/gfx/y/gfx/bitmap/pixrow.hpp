

//! \file

#ifndef Y_GFX_BITMAP_LINES_INCLUDED
#define Y_GFX_BITMAP_LINES_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace GFX
    {

        class PixRow
        {
        public:
            PixRow(void        *entry,
                   const unit_t width) throw();
            ~PixRow() throw();

            void *       const p;
            const unit_t       w;

            void       *at(const unit_t i, const unit_t bpp)       throw();
            const void *at(const unit_t i, const unit_t bpp) const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixRow);
        };

        class PixRows
        {
        public:
            PixRows(const unit_t width,
                    const unit_t height,
                    const void  *entry,
                    const unit_t stride);
            ~PixRows() throw();

            const unit_t  h;

            PixRow       & operator[](const unit_t j) throw();
            const PixRow & operator[](const unit_t j) const throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixRows);
            size_t  count;
            size_t  bytes;
            PixRow *row;

        };

    }

}

#endif
