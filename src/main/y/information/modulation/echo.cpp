#include "y/information/modulation/echo.hpp"

namespace upsylon {

    namespace information {

        echo_modulation::~echo_modulation() throw() {}


        echo_modulation:: echo_modulation() throw()  {}

        void echo_modulation:: reset() throw() {   }


        uint8_t echo_modulation:: fetch(const uint8_t c) throw()
        {
            return c;
        }


        Y_INFORMATION_MODULATION(echo_modulation);

        modulation * echo_modulation:: clone() const  
        {
            return new echo_modulation();
        }

    }


}
