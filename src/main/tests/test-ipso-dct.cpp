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
        ipso::DCT::Table p("p",w,w);

        sq.forward(f,im,1,1);    sq.reverse(p,f);
        sq.forward(f,fm,10,10);  sq.reverse(p,f);
        sq.forward(f,ff,30,30);  sq.reverse(p,f);

        for(unit_t h=1;h<=16;++h)
        {
            ipso::CommonDCT  dct(w,h);
            ipso::DCT::Table g("w",w,h);
            ipso::DCT::Table q("q",w,h);

            dct.forward(g,im,1,1);   dct.reverse(q,g);
            dct.forward(g,fm,10,10); dct.reverse(q,g);
            dct.forward(g,ff,30,30); dct.reverse(q,g);
        }
    }
}
Y_UTEST_DONE()

