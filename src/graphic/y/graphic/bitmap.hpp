//! \file

#ifndef Y_GRAPHIC_BITMAP_INCLUDED
#define Y_GRAPHIC_BITMAP_INCLUDED 1

#include "y/graphic/metrics.hpp"
#include "y/graphic/memory.hpp"


namespace upsylon {

    namespace Graphic {


        class Bitmap : public Metrics, public Memory
        {
        public:

            virtual ~Bitmap() throw();
            explicit Bitmap(const size_t W,
                            const size_t H,
                            const size_t D,
                            const Memory::Mode = RW);

            



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);
            
        };

    }
}


#endif


