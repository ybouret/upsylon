
#include "y/type/fourcc.hpp"

namespace upsylon {

    const char *fourcc_(const uint32_t dw) throw()
    {
        static char buf[8];


        buf[0] = Y_FOURCC_AT(0,dw);
        buf[1] = Y_FOURCC_AT(1,dw);
        buf[2] = Y_FOURCC_AT(2,dw);
        buf[3] = Y_FOURCC_AT(3,dw);
        buf[4] = buf[5] = buf[6] = buf[7] = 0;

        return buf;
    }



}
