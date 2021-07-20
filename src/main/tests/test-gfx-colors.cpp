
#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"
#include "y/ios/align.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T, typename U>
    static inline void  Check(const T &value)
    {
        std::cerr << "convert<" << rtti::name_of<T>() << "," << rtti::name_of<U>() << ">::from" << std::endl;
        std::cerr << "\t" << value << " -> " << convert<U,T>::from(value) << std::endl;
    }

    template <typename T>
    void CheckAll(const T &value)
    {
        Check<T,uint8_t>(value);
        Check<T,float>(value);
        Check<T,rgb>(value);
        Check<T,rgba>(value);
        Check<T,YUV>(value);
        std::cerr << std::endl;
    }
}

Y_UTEST(gfx_colors)
{

    {
        const uint8_t     u = 'A';
        CheckAll<uint8_t>(u);
    }

    {
        const float     f = 0.22f;
        CheckAll<float>(f);
    }

    {
        const rgb     c(50,100,150);
        CheckAll<rgb>(c);
    }

    {
        const rgba     c(50,100,150);
        CheckAll<rgba>(c);
    }

    {
        const YUV   yuv(0.5f,0.1f,-0.1f);
        CheckAll<YUV>(yuv);
    }

    {
        for(size_t i=0;i<8;++i)
        {
            const rgb C( alea.full<uint8_t>(), alea.full<uint8_t>(), alea.full<uint8_t>() );

            C.saturated();
        }
    }


}
Y_UTEST_DONE()

