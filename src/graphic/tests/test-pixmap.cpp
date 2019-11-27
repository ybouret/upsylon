
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/stack.hpp"
#include "y/utest/run.hpp"
#include "y/hashing/sha1.hpp"
#include "y/sequence/list.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace Graphic;


namespace    {

#define ALEA_COORD (1+alea.leq(200))

    template <typename T>
    static inline void doPxm( Pixmap<T> &pxm )
    {
        std::cerr << "Pixmap<" << typeid(T).name() << ">: depth=" << pxm->depth << std::endl;
        std::cerr << (Area&)(*pxm)<< std::endl;

        const Pixmap<T> &cxm = pxm;
        for(unit_t j=0;j<pxm->h;++j)
        {
            for(unit_t i=0;i<pxm->w;++i)
            {

                (void) pxm[j][i];
                (void) pxm(j)(i);

                (void) cxm[j][i];
                (void) cxm(j)(i);
            }
        }

    }
}


Y_UTEST(pixmap)
{
    Pixmap1          pxm1(ALEA_COORD,ALEA_COORD);
    Pixmap<uint16_t> pxm2(ALEA_COORD,ALEA_COORD);
    Pixmap<uint32_t> pxm4(ALEA_COORD,ALEA_COORD);
    Pixmap<uint64_t> pxm8(ALEA_COORD,ALEA_COORD);
    Pixmap3          rgb(ALEA_COORD,ALEA_COORD);
    Pixmap4          rgba(ALEA_COORD,ALEA_COORD);


    doPxm(pxm1);
    doPxm(pxm2);
    doPxm(pxm4);
    doPxm(pxm8);
    doPxm(rgb);
    doPxm(rgba);

}
Y_UTEST_DONE()

