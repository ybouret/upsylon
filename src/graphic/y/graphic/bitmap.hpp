//! \file

#ifndef Y_GRAPHIC_BITMAP_INCLUDED
#define Y_GRAPHIC_BITMAP_INCLUDED 1

#include "y/graphic/metrics.hpp"
#include "y/graphic/memory.hpp"
#include "y/graphic/rectangle.hpp"

namespace upsylon {

    namespace Graphic {


        class AnonymousRow
        {
        public:
            void        *p;
            const unit_t w;
            const unit_t ww;
            unit_t       indexOf(const unit_t i) const throw();

        private:
            AnonymousRow(); ~AnonymousRow() throw();
            Y_DISABLE_COPY_AND_ASSIGN(AnonymousRow);
        };

        class Bitmap : public Memory, public Area
        {
        public:
            explicit Bitmap( Metrics &metrics );
            virtual ~Bitmap() throw();

            Bitmap( const Bitmap &bmp );                       //!< share memory, create rows
            Bitmap( const Bitmap &bmp, const Rectangle &rect); //!< share memory, create rows


            const unit_t hh;
            const size_t depth;
            const size_t scanline;
            const size_t stride;

            const void *get(const unit_t i, const unit_t j) const throw();

            void clear() throw();

        protected:
            AnonymousRow    *rows;

        private:
            Y_DISABLE_ASSIGN(Bitmap);
            size_t  __rnum;
            size_t  __rlen;
            void setup(void *origin);

        public:
            const AnonymousRow *row(const unit_t j) const throw();
        };

    }
}


#endif


