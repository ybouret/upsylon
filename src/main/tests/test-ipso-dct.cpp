#include "y/ipso/dct.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"

using namespace upsylon;

Y_UTEST(ipso_dct)
{
    matrix<int>           im(128,128);
    matrix<float>         fm(60,60);
    ipso::field2D<double> ff("ff",100,100);

    for(unit_t w=1;w<=16;++w)
    {
        ipso::SquareDCT  sq(w);
        ipso::DCT::Table f("f",w,w);
        sq.forward(f,im,1,1);
        sq.forward(f,fm,10,10);
        sq.forward(f,ff,30,30);

        for(unit_t h=1;h<=16;++h)
        {
            ipso::CommonDCT dct(w,h);
            ipso::DCT::Table g("w",w,h);
            dct.forward(g,im,1,1);
            dct.forward(g,fm,10,10);
            dct.forward(g,ff,30,30);
        }
    }
}
Y_UTEST_DONE()

