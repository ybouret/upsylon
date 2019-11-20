//! \file

#ifndef Y_GRAPHIC_BITMAP_INCLUDED
#define Y_GRAPHIC_BITMAP_INCLUDED 1

#include "y/graphic/metrics.hpp"
#include "y/graphic/memory.hpp"


namespace upsylon {

    namespace Graphic {


        class Bitmap : public Memory, public Area
        {
        public:
            explicit Bitmap( Metrics &metrics );
            virtual ~Bitmap() throw();

            const size_t depth;
            const size_t scanline;
            const size_t stride;

        protected:
            struct Row
            {
                void  *p;
                size_t w;
            };
            Row    *rows;

        private:
            size_t  __rnum;
            size_t  __rlen;
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
            void setup();

        };

    }
}


#endif


