//! \file
#ifndef Y_INFORMATION_ECHO_INCLUDED
#define Y_INFORMATION_ECHO_INCLUDED 1

#include "y/information/shaker.hpp"

namespace upsylon {

    namespace information {

        //! base class for simple echoing
        class echo_modulation : public shaker
        {
        public:
            explicit        echo_modulation()    throw(); //!< setup
            virtual        ~echo_modulation()    throw(); //!< cleanup
            virtual void    reset()              throw(); //!< do nothing
            virtual uint8_t fetch(const uint8_t) throw(); //!< do nothing
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(echo_modulation);
        };

    }

}

#endif

