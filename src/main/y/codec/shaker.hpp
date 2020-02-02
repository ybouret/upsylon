//! \file
#ifndef Y_INFORMATION_SHAKER_INCLUDED
#define Y_INFORMATION_SHAKER_INCLUDED 1

#include "y/memory/buffer.hpp"

namespace upsylon {

    namespace information {

        //! basic information shaker interface
        class shaker
        {
        public:
            virtual ~shaker() throw(); //!< cleanup

            virtual void    reset()              throw() = 0; //!< restart state
            virtual uint8_t fetch(const uint8_t) throw() = 0; //!< byte wise process

        protected:
            explicit shaker() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(shaker);
        };
    }


}

#endif

