#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

namespace
{
    template <const size_t BYTES>
    void xswap(void *a,void *b) throw()
    {
        struct mocking { char data[BYTES]; };
        const mocking tmp = *(mocking *)a;
        *(mocking *)a = *(mocking *)b;
        *(mocking *)b = tmp;
    }
}

namespace
{
#if defined(NDEBUG)
    static const char fn[] = "bswap.dat";
#else
    static const char fn[] = "bswapd.dat";
#endif

    static inline
    void init(void *a,void *b,const size_t n) throw()
    {
        uint8_t *p = (uint8_t *)a;
        uint8_t *q = (uint8_t *)b;
        uint8_t  v = 0;
        for(size_t i=0;i<n;++i)
        {
            p[i] = 0xff;
            q[i] = v++;
        }
    }

    static inline
    void check(const void *a,const void *b,const size_t n)
    {
        const uint8_t *p = (uint8_t *)a;
        const uint8_t *q = (uint8_t *)b;
        uint8_t  v = 0;
        for(size_t i=0;i<n;++i,++v)
        {
            Y_ASSERT(0xff==q[i]);
            Y_ASSERT(v==p[i]);
        }
    }

    template <const size_t N>
    static inline
    void swp(void *a, void *b, const double D)
    {
        std::cerr << "swp<" << N << ">" << std::endl;


        double b_speed = 0;
        init(a,b,N);
        core::bswap<N>(a,b);
        check(a,b,N);
        Y_TIMINGS(b_speed, D, core::bswap<N>(a,b));
        b_speed *= 1e-6;
        std::cerr << "\tbswap: " << b_speed << std::endl;

        double m_speed = 0;
        if(N>0)
        {
            init(a,b,N);
            mswap(a,b,N);
            check(a,b,N);
            Y_TIMINGS(m_speed,D,mswap(a,b,N));
        }
        m_speed *= 1e-6;
        std::cerr << "\tmswap: " << m_speed << std::endl;

        double x_speed = 0;
        if(N>0)
        {
            init(a,b,N);
            xswap<N>(a,b);
            check(a,b,N);
            Y_TIMINGS(x_speed,D, xswap<N>(a,b));
        }
        x_speed *= 1e-6;
        std::cerr << "\txswap: " << x_speed << std::endl;

        if(N>0)
        {
            ios::ocstream::echo(fn, "%lu %g %g %g\n", (unsigned long)N, b_speed, m_speed, x_speed);
        }
    }

}

using namespace upsylon;

#define SWP(N) swp<N>(a,b,D)

Y_UTEST(bswap)
{
    double D = 0.02;
    if(argc>1)
    {
        D = string_convert::to<double>(argv[1]);
    }

    double u = 10;
    double v = 20;
    bswap(u,v);
    std::cerr << u << " " << v << std::endl;

    const size_t nmax = 256;
    memory::cblock_of<char> blk(2*nmax);
    char *a = blk.data;
    char *b = blk.data+nmax;


    ios::ocstream::overwrite(fn);

    SWP(0x00); SWP(0x01);  SWP(0x02); SWP(0x03); SWP(0x04); SWP(0x05); SWP(0x06); SWP(0x07);
    SWP(0x08); SWP(0x09);  SWP(0x0A); SWP(0x0B); SWP(0x0C); SWP(0x0D); SWP(0x0E); SWP(0x0F);

    SWP(0x10); SWP(0x11);  SWP(0x12); SWP(0x13); SWP(0x14); SWP(0x15); SWP(0x16); SWP(0x17);
    SWP(0x18); SWP(0x19);  SWP(0x1A); SWP(0x1B); SWP(0x1C); SWP(0x1D); SWP(0x1E); SWP(0x1F);

    SWP(0x20); SWP(0x21);  SWP(0x22); SWP(0x23); SWP(0x24); SWP(0x25); SWP(0x26); SWP(0x27);
    SWP(0x28); SWP(0x29);  SWP(0x2A); SWP(0x2B); SWP(0x2C); SWP(0x2D); SWP(0x2E); SWP(0x2F);

    SWP(0x30); SWP(0x31);  SWP(0x32); SWP(0x33); SWP(0x34); SWP(0x35); SWP(0x36); SWP(0x37);
    SWP(0x38); SWP(0x39);  SWP(0x3A); SWP(0x3B); SWP(0x3C); SWP(0x3D); SWP(0x3E); SWP(0x3F);

    SWP(0x40);
    SWP(80);
    SWP(128);




}
Y_UTEST_DONE()

