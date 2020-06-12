#include "y/spade/algo/dct.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Spade;

Y_UTEST(dct)
{
    static const unit_t w_max = 16;
    static const unit_t h_max = w_max;
    static const unit_t w_add = 5;
    static const unit_t h_add = 8;


    for(unit_t w=1;w<=w_max;++w)
    {
        std::cerr << "w=" << w << std::endl;
        for(unit_t h=1;h<=h_max;++h)
        {
            std::cerr << "h=" << h << "|";
            CommonDCT      dct(w,h);
            DCT::Table     target("target",w,h);
            DCT::Table     pix("pix",w,h);
            Field2D<float> source("source",w+w_add,h+h_add);
            DCT::Loop      loop(source.lower,source.upper);
            for(loop.boot();loop.good();loop.next())
            {
                source[ *loop ] = alea.to<float>();
            }

            for(unit_t xx=0;xx<w_add;++xx)
            {
                for(unit_t yy=0;yy<h_add;++yy)
                {
                    dct.forward(target,source, xx, yy);
                    dct.reverse(pix,target);
                }
            }
        }
        std::cerr << std::endl;
    }
}
Y_UTEST_DONE()


