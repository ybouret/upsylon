
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace ink;

namespace
{
    template <typename T>
    static inline void do_test(imageIO &img,const string &id)
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

        const string root = "img" + id + ".";
        img.save(root+"jpg", pxm,NULL);
        img.save(root+"png", pxm,NULL);
        img.save(root+"tif", pxm,NULL);

    }

}

Y_UTEST(image)
{
    image   &__img = image::instance().initialize();
    imageIO &img   = __img;

    __img.display();
    do_test<float>(img,"f");
    do_test<uint8_t>(img,"u");
    do_test<RGB>(img,"3");
    do_test<RGBA>(img,"4");
    //do_test<cplx>();

}
Y_UTEST_DONE()
