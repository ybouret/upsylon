
#include "y/graphic/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/hashing/sha1.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;
using namespace Graphic;


namespace    {

#define ALEA_COORD (1+alea.leq(200))

    template <typename T>
    static inline void doPxm( Pixmap<T> &pxm )
    {
        for(unit_t j=0;j<pxm->h;++j)
        {
            for(unit_t i=0;i<pxm->w;++i)
            {
                (void) pxm[j][i];
            }
        }

    }
}


Y_UTEST(pixmap)
{
    Pixmap<uint8_t> pxm1(ALEA_COORD,ALEA_COORD);

    doPxm(pxm1);

}
Y_UTEST_DONE()

