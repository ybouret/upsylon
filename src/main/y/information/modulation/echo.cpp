#include "y/information/modulation/echo.hpp"

namespace upsylon {

    namespace Information {

        EchoModulation::~EchoModulation() throw() {}


        EchoModulation:: EchoModulation() throw()  {}

        void EchoModulation:: reset() throw() {   }


        uint8_t EchoModulation:: fetch(const uint8_t c) throw()
        {
            return c;
        }


        Y_INFORMATION_MODULATION(EchoModulation);

        Modulation * EchoModulation:: clone() const
        {
            return new EchoModulation();
        }

    }


}
