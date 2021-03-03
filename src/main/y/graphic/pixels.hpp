//! \file

#ifndef Y_GRAPHIC_PIXELS_INCLUDED
#define Y_GRAPHIC_PIXELS_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! anonymous pixels
        //
        //______________________________________________________________________
        class pixels : public object, public counted
        {
        public:
            virtual ~pixels() throw();               //!< release memory
            explicit pixels(const size_t required);  //!< allocate block_size >= required

            const size_t block_size; //!< 2^block_exp2 bytes
            const size_t block_exp2; //!< 0..
            void        *block_addr; //!< allocated address


        private:
            Y_DISABLE_COPY_AND_ASSIGN(pixels);
        };
        

    }

}
#endif

