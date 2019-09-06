#include "y/oxide/algo/dct.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"

using namespace upsylon;


template <typename T>
void fillArr( T *p, size_t n)
{
    while(n-->0)
    {
        *(p++) = alea.range<int>(-10,10);
    }
}

Y_UTEST(oxide_dct)
{
    // worksapces
    matrix<int>            im(128,128);
    matrix<float>          fm(60,60);
    Oxide::Field2D<double> ff("ff",100,100);
    std::cerr << "ff: " << ff << std::endl;

    fillArr( &im[1][1], im.items);
    fillArr( &fm[1][1], fm.items);
    fillArr( ff.entry,  ff.items);


    for(unit_t w=1;w<=16;++w)
    {
        Oxide::SquareDCT  sq(w);
        Oxide::DCT::Table f("f",w,w);
        Oxide::DCT::Table p("p",w,w);

        sq.forward(f,im,1,1);    sq.reverse(p,f);
        sq.forward(f,fm,10,10);  sq.reverse(p,f);
        sq.forward(f,ff,30,30);  sq.reverse(p,f);

        for(unit_t h=1;h<=16;++h)
        {
            Oxide::CommonDCT  dct(w,h);
            Oxide::DCT::Table g("w",w,h);
            Oxide::DCT::Table q("q",w,h);

            dct.forward(g,im,1,1);   dct.reverse(q,g);
            dct.forward(g,fm,10,10); dct.reverse(q,g);
            dct.forward(g,ff,30,30); dct.reverse(q,g);
        }
    }
}
Y_UTEST_DONE()

