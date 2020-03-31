
#include "y/type/fourcc.hpp"
#include "y/code/utils.hpp"
#include <cstring>


namespace upsylon {

    const char *fourcc_(const uint32_t dw) throw()
    {
        static char buf[32];
        memset(buf,0,sizeof(buf));
        //strcat(buf, "hello");
        
#define Y_FCC_CAT(I) strcat(buf, cchars::encoded[ Y_FOURCC_AT(I,dw) ] )
        Y_FCC_CAT(0);
        Y_FCC_CAT(1);
        Y_FCC_CAT(2);
        Y_FCC_CAT(3);

        //buf[0] = Y_FOURCC_AT(0,dw);
        //buf[1] = Y_FOURCC_AT(1,dw);
        //buf[2] = Y_FOURCC_AT(2,dw);
        //buf[3] = Y_FOURCC_AT(3,dw);
        
        return buf;
    }



}
