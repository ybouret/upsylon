
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace Ink;

namespace
{

    struct genPixel
    {
        template <typename T> static T get();
    };

    template <> float   genPixel::get<float>()     { return alea.to<float>();     }
    template <> uint8_t genPixel::get<uint8_t>()   { return alea.full<uint8_t>(); }
    template <> RGB     genPixel::get<RGB>()       { return RGB(alea.full<uint8_t>(),alea.full<uint8_t>(),alea.full<uint8_t>()); }
    template <> RGBA    genPixel::get<RGBA>()      { return RGBA(alea.full<uint8_t>(),alea.full<uint8_t>(),alea.full<uint8_t>(),alea.full<uint8_t>()); }

    template <typename T>
    static inline void do_test(ImageIO &img,const string &id)
    {
        Pixmap<T> pxm( 100+alea.leq(400), 100+alea.leq(400) );
        for(size_t y=0;y<pxm.h;++y)
        {
            for(size_t x=0;x<pxm.w;++x)
            {
                pxm[y][x] = genPixel::get<T>();
            }
        }
        std::cerr << "Pixmap<" << typeid(T).name() << "> " << pxm.w << 'x' << pxm.h << " => depth=" << pxm.depth << ", bytes=" << pxm.bytes << std::endl;

        const string root = "img" + id + ".";
        img.save(root+"jpg", pxm,NULL);
        img.save(root+"png", pxm,NULL);
        img.save(root+"tif", pxm,NULL);

    }

}

Y_UTEST(image)
{
    Image   &__img = Image::Init();
    ImageIO &img   = __img;

    __img.display();
    do_test<float>(img,"f");
    do_test<uint8_t>(img,"u");
    do_test<RGB>(img,"3");
    do_test<RGBA>(img,"4");

}
Y_UTEST_DONE()
