#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{

    namespace core
    {

        template <const size_t N>
        void Bswap(void *,void *) throw();

        template <> void Bswap<0>(void*,void*) throw(){}

    }

    template <typename T>
    void Bswap( T &a, T &b ) throw()
    {
        core::Bswap<sizeof(T)>( &a, &b );
    }


#define Y_MSWAP_BYTE(I) const uint8_t tmp = P[I]; P[I] = Q[I]; Q[I] = tmp
    inline void Mswap(void *p,void *q, const size_t n) throw()
    {
        assert(n>0);
        uint8_t *P = static_cast<uint8_t *>(p);
        uint8_t *Q = static_cast<uint8_t *>(q);
        Y_LOOP_FUNC_(n,Y_MSWAP_BYTE,0);
    }

}

using namespace upsylon;

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
            p[i] = 0;
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
            Y_ASSERT(0==q[i]);
            Y_ASSERT(v==p[i]);
        }
    }

    template <const size_t N>
    static inline
    void swp(void *a, void *b, const double D)
    {
        std::cerr << "swp<" << N << ">" << std::endl;
        init(a,b,N);
        double m_speed = 0;
        if(N>0)
        {
            Mswap(a,b,N);
            check(a,b,N);
            Y_TIMINGS(m_speed,D,Mswap(a,b,N));
        }
        m_speed *= 1e-6;
        std::cerr << "\tmswap: " << m_speed << std::endl;
        //double b_speed = 0;
        //Y_TIMINGS(b_speed, D, core::Bswap<N>(a,b));

    }

}

using namespace upsylon;

#define SWP(N) swp<N>(a,b,D)

Y_UTEST(bswap)
{
    double D = 0.1;
    const size_t nmax = 64;
    memory::cblock_of<char> blk(2*nmax);
    char *a = blk.data;
    char *b = blk.data+nmax;

    ios::ocstream::overwrite(fn);

    SWP(0x00); SWP(0x01);  SWP(0x02); SWP(0x03); SWP(0x04); SWP(0x05); SWP(0x06); SWP(0x07);
    SWP(0x08); SWP(0x09);  SWP(0x0A); SWP(0x0B); SWP(0x0C); SWP(0x0D); SWP(0x0E); SWP(0x0F);


}
Y_UTEST_DONE()

