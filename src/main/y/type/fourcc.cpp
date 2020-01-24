
#include "y/type/fourcc.hpp"

namespace upsylon {

    const char *fourcc_(const uint32_t dw) throw()
    {
        static char ans[8];


        ans[0] = Y_FOURCC_AT(0,dw);
        ans[1] = Y_FOURCC_AT(1,dw);
        ans[2] = Y_FOURCC_AT(2,dw);
        ans[3] = Y_FOURCC_AT(3,dw);
        ans[4] = ans[5] = ans[6] = ans[7] = 0;

        return ans;
    }
}
