//! \file

#ifndef Y_GRAPHIC_PIXELS_INCLUDED
#define Y_GRAPHIC_PIXELS_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! anonymous pixels
        class pixels : public object, public counted
        {
        public:
            virtual ~pixels() throw();
            explicit pixels(const size_t required);

            const size_t block_size;
            const size_t block_exp2;
            void        *block_addr;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(pixels);
        };

    }

}
#endif

