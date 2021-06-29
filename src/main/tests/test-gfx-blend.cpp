#include "y/gfx/pixel/interpolation.hpp"
#include "y/utest/run.hpp"
#include <cstdio>
#include "y/type/utils.hpp"
#include "y/gfx/color/named.hpp"
#include "y/ios/ocstream.hpp"
#include "y/type/aliasing.hpp"
#include "y/sequence/vector.hpp"
#include <iomanip>

using namespace upsylon;
using namespace graphic;

namespace {

    static inline void display_table()
    {
        std::cout.flush();

        size_t count=1;
        for(int i=-255;i<=255;++i,++count)
        {
            printf("%4d,", i);
            if(0==(++count%32)) printf("\n");
        }
        printf(" 0\n");
    }

    static inline bool check_blend(const float w, const int fg, const int bg)
    {
        const uint8_t cmin = min_of(fg,bg);
        const uint8_t cmax = max_of(fg,bg);
        const uint8_t cmix = blend<float,uint8_t>::mix(w,uint8_t(fg),uint8_t(bg));
        return cmin<=cmix && cmix <= cmax;
    }

    static inline bool within(const uint8_t a, const uint8_t b, const uint8_t c) throw()
    {
        return min_of(a,b) <= c && c <= max_of(a,b);
    }

}


Y_UTEST(gfx_blend)
{
    if(true)         display_table();
    std::cerr << "checking table" << std::endl;
    for(int i=-255;i<=255;++i)
    {
        Y_ASSERT( (blend<uint8_t,uint8_t>::shift[i] == i) );
    }

    std::cerr << "checking integer mix..." << std::endl;
    for(int fg=0;fg<256;++fg)
    {
        for(int bg=0;bg<256;++bg)
        {
            Y_ASSERT(check_blend(0,fg,bg));
            Y_ASSERT(check_blend(1,fg,bg));
            for(size_t iter=0;iter<1024;++iter)
            {
                Y_ASSERT(check_blend(alea.to<float>(),fg,bg));
            }
        }
    }

    std::cerr << "checking rgb mix..." << std::endl;

    for(int i=0;i<Y_GFX_NAMED_COLORS;++i)
    {
        const rgb a = named_color::table[i];
        for(int j=0;j<Y_GFX_NAMED_COLORS;++j)
        {
            const rgba b = named_color::table[j];
            for(size_t iter=0;iter<128;++iter)
            {
                const rgb ab = blend<float,rgba>::mix(alea.to<float>(),a,b);
                Y_ASSERT( within(a.r,b.r,ab.r) );
                Y_ASSERT( within(a.g,b.g,ab.g) );
                Y_ASSERT( within(a.b,b.b,ab.b) );
            }
        }
    }

    std::cerr << "checking interp..." << std::endl;

    {
        ios::ocstream fp("interp.dat");
        uint8_t               arr[] = { 10,20,30,40,50,60,70,80 };
        static const unsigned num = sizeof(arr)/sizeof(arr[0]);
        for(unsigned i=0;i<=100;++i)
        {
            const float x = i/100.0f;
            fp("%g %u %u\n",x,interp::closest_<uint8_t,num>(x,arr), interp::linear_<uint8_t,num>(x,arr) );
        }
    }

    {
        const float   x[] = { 0.0f, 0.2f, 0.7f, 1.0f };
        const uint8_t y[] = {   10,   20,   30,   40 };
        {
            ios::ocstream fp("linear-src.dat");
            for(unsigned i=0;i<sizeof(x)/sizeof(x[0]);++i)
            {
                fp("%g %u\n",x[i],unsigned(y[i]));
            }
        }
        {

            ios::ocstream fp("linear-int.dat");
            for(unsigned i=0;i<=100;++i)
            {
                size_t      j  = 0;
                const float xx = i/100.0f;
                fp("%g %u\n", xx, interp::linear<uint8_t,const float * const,const uint8_t * const>(xx,  aliasing::prev(x), aliasing::prev(y), sizeof(x)/sizeof(x[0]), j) );
            }
        }
    }


    {
        vector<rgba> colors;
        colors << Y_RED;
        colors << Y_BLUE;
        for(float xx=0;xx<=1;xx+=0.2f)
        {
            const rgba C = interp::linear(xx,colors);
            std::cerr << "x=" << std::setw(4) << xx << " => " << C << std::endl;
        }
    }


}
Y_UTEST_DONE()
