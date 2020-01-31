
#include "y/type/fourcc.hpp"
#include "y/type/bzset.hpp"
#include <cstring>

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

    fourcc:: fourcc( const uint32_t dw ) throw() :
    uuid(dw),
    clid()
    {
        char *buf = (char *)clid;
        buf[0] = Y_FOURCC_AT(0,dw);
        buf[1] = Y_FOURCC_AT(1,dw);
        buf[2] = Y_FOURCC_AT(2,dw);
        buf[3] = Y_FOURCC_AT(3,dw);
        buf[4] = buf[5] = buf[6] = buf[7] = 0;
    }

    fourcc:: fourcc( const fourcc &other ) throw() :
    uuid(other.uuid),
    clid()
    {
        memcpy((void*)clid,other.clid,sizeof(clid));
    }

    fourcc:: ~fourcc() throw()
    {
        _bzset(uuid);
        memset((void*)clid,0,sizeof(clid));
    }

}
