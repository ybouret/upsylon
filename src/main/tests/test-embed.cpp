#include "y/memory/embed.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/global.hpp"
#include "support.hpp"

using namespace upsylon;

template <typename T>
static inline void fill( T *p, size_t n )
{
    while(n-->0)
    {
        p[n] = support::get<T>();
    }
}

Y_UTEST(embed)
{
    float       *F = NULL;
    const size_t nF = 4;

    double      *D = NULL;
    const size_t nD = 7;

    int         *I = NULL;
    const size_t nI = 10;

    uint16_t      *U  = NULL;
    const size_t   nU = 3;

    void   *wksp = 0;
    size_t  wlen = 0;
    {
        memory::embed emb[] =
        {
            memory::embed::as(F,nF),
            memory::embed::as(D,nD),
            memory::embed::as(I,nI),
            memory::embed::as(U,nU)
        };
        wksp = memory::embed::create_global(emb, sizeof(emb)/sizeof(emb[0]), wlen);
    }

    fill(F,nF);
    fill(D,nD);
    fill(I,nI);
    fill(U,nU);
    
    memory::global::location().release(wksp, wlen);

}
Y_UTEST_DONE()

