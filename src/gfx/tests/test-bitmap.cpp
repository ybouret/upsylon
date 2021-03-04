

#include "y/gfx/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(bitmap)
{
    Y_UTEST_SIZEOF(area);
    Y_UTEST_SIZEOF(bitrow);
    Y_UTEST_SIZEOF(bitrows);
    Y_UTEST_SIZEOF(crux::pixels);

    for(unit_t depth=1;depth<=4;++depth)
    {
        for(unit_t w=1;w<=100;w+=1+alea.leq(10))
        {
            for(unit_t h=1;h<=100;h+=1+alea.leq(10))
            {
                bitmap bmp(w,h,depth);
                std::cerr << bmp << std::endl;
                const bitmap cpy = bmp;
                std::cerr << cpy << std::endl;

            }
        }

    }



}
Y_UTEST_DONE()

