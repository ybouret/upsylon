
#include "y/ink/pixmaps.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace ink;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        pixmap<T> pxm( 1+alea.leq(100), 1+alea.leq(100) );
        for(unit_t y=0;y<pxm.h;++y)
        {
            for(unit_t x=0;x<pxm.w;++x)
            {
                pxm[y][x] = pixel<T>::zero;
            }
        }
        std::cerr << "pixmap<" << typeid(T).name() << "> " << pxm.sizes << " => depth=" << pxm.depth << ", bytes=" << pxm.bytes << std::endl;
    }

}

Y_UTEST(pixmaps)
{
    do_test<float>();
    do_test<uint8_t>();
    do_test<RGB>();
    do_test<RGBA>();
    do_test<cplx>();

}
Y_UTEST_DONE()
