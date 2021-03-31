#include "y/gfx/blend.hpp"
#include "y/utest/run.hpp"
#include <cstdio>
#include "y/type/utils.hpp"
#include "y/gfx/color/named.hpp"

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
        const uint8_t cmix = blend_::mixf(w,uint8_t(fg),uint8_t(bg));
        return cmin<=cmix && cmix <= cmax;
    }

    static inline bool within(const uint8_t a, const uint8_t b, const uint8_t c) throw()
    {
        return min_of(a,b) <= c && c <= max_of(a,b);
    }

}


Y_UTEST(blend)
{
    if(true)         display_table();
    std::cerr << "checking table" << std::endl;
    for(int i=-255;i<=255;++i)
    {
        Y_ASSERT( blend_::ishift[i] == i );
    }

    std::cerr << "checking mixf..." << std::endl;
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

    std::cerr << "checking mix..." << std::endl;

    for(int i=0;i<Y_GFX_NAMED_COLORS;++i)
    {
        const rgb a = named_color::table[i];
        for(int j=0;j<Y_GFX_NAMED_COLORS;++j)
        {
            const rgba b = named_color::table[j];
            for(size_t iter=0;iter<128;++iter)
            {
                const rgb ab = blend_::mix(alea.to<float>(),a,b);
                Y_ASSERT( within(a.r,b.r,ab.r) );
                Y_ASSERT( within(a.g,b.g,ab.g) );
                Y_ASSERT( within(a.b,b.b,ab.b) );
            }
        }
    }
    



}
Y_UTEST_DONE()
