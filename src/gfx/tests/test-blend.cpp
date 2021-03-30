#include "y/gfx/blend.hpp"
#include "y/utest/run.hpp"
#include <cstdio>
#include "y/type/utils.hpp"

using namespace upsylon;
using namespace graphic;

namespace {

    static inline void display_table()
    {
        std::cout.flush();

        size_t count=1;
        for(int i=-255;i<=255;++i,++count)
        {
            printf("%4d.0f,", i);
            if(0==(++count%32)) printf("\n");
        }
        printf(" 0.0f\n");
    }

    static inline bool check_blend(const float w, const int fg, const int bg)
    {
        const uint8_t cmin = min_of(fg,bg);
        const uint8_t cmax = max_of(fg,bg);
        const uint8_t cmix = blend::mix(w,uint8_t(fg),uint8_t(bg));
        return cmin<=cmix && cmix <= cmax;
    }

}


Y_UTEST(blend)
{
    if(false) display_table();
    std::cerr << "checking table" << std::endl;
    for(int i=-255;i<=255;++i)
    {
        Y_ASSERT( blend::shift[i] == i );
    }

    std::cerr << "checking mix..." << std::endl;
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


}
Y_UTEST_DONE()
