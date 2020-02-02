
#include "y/codec/echo.hpp"

namespace upsylon {

    echo_encoding::~echo_encoding() throw() {}


    echo_encoding:: echo_encoding() throw()  {}

    void echo_encoding:: reset() throw() {   }


    uint8_t echo_encoding:: encode(uint8_t c) throw()
    {
        return c;
    }

    uint8_t echo_encoding:: decode(uint8_t c) throw()
    {
        return c;
    }


}
