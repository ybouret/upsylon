
#include "y/ink/pixmaps.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace Ink;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        Pixmap<T> pxm( 1+alea.leq(100), 1+alea.leq(100) );
        for(size_t y=0;y<pxm.h;++y)
        {
            for(size_t x=0;x<pxm.w;++x)
            {
                pxm[y][x] = Pixel<T>::Zero;
            }
        }
        std::cerr << "Pixmap<" << typeid(T).name() << "> " << pxm.w << 'x' << pxm.h << " => depth=" << pxm.depth << ", bytes=" << pxm.bytes << std::endl;
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
