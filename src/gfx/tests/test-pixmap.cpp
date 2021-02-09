

#include "y/gfx/pixmap.hpp"
#include "y/utest/run.hpp"
#include "../../main/tests/support.hpp"

using namespace upsylon;
using namespace GFX;

namespace {

    template <typename T>
    static inline void testPxm()
    {
        for(unit_t w=1;w<=100; w += 10 + alea.leq(10) )
        {
            for(unit_t h=1;h<=100; h += 10 + alea.leq(10) )
            {

                Pixmap<T> pxm(w,h);
                std::cerr << pxm << std::endl;
                pxm.ldz();
                for(unit_t j=0;j<h;++j)
                {
                    for(unit_t i=0;i<w;++i)
                    {
                        pxm[j][i] = support::get<T>();
                    }
                }
            }

        }

    }
}

Y_UTEST(pixmap)
{

    testPxm<float>();
    testPxm<double>();
    testPxm< complex<double> >();
}
Y_UTEST_DONE()
