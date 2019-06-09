#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/memory/cblock.hpp"
#include "y/ios/ocstream.hpp"
#include <cstring>
#include "support.hpp"

using namespace upsylon;

namespace
{


    template <const size_t BYTES, typename T, typename U>
    inline void __bmove(T *target, const U *source)
    {
        struct mocking { char data[BYTES]; };
        *(reinterpret_cast<mocking *>(target)) =   *(reinterpret_cast<const mocking *>(source));
        char       *t = (char *) target;
        const char *s = (const char *) source;
        for(size_t i=0;i<BYTES;++i)
        {
            Y_ASSERT(s[i]==t[i]);
        }
    }




    namespace core
    {
        template <const size_t BYTES>
        inline void bmove(void *a, const void *b)
        {
            struct mocking { char data[BYTES]; };
            *(reinterpret_cast<mocking *>(a)) =   *(reinterpret_cast<const mocking *>(b));
        }

        template <>
        inline void bmove<0>(void *,const void *)
        {
        }

    }

    static inline void init(void *a, void *b, const size_t nmax)
    {
        uint8_t *A = static_cast<uint8_t *>(a);
        uint8_t *B = static_cast<uint8_t *>(b);
        uint8_t  value = 0;
        for(size_t i=0;i<nmax;++i)
        {
            A[i] = 0;
            B[i] = ++value;
        }
    }

    static inline void checkMoved(const void *a, const size_t nmax)
    {
        const uint8_t *A = static_cast<const uint8_t *>(a);
        uint8_t value = 0;
        for(size_t i=0;i<nmax;++i)
        {
            Y_ASSERT(++value==A[i]);
        }
    }

    static inline void checkMoved(const void *a, const void *b, const size_t nmax)
    {
        const uint8_t *A = static_cast<const uint8_t *>(a);
        const uint8_t *B = static_cast<const uint8_t *>(b);

        for(size_t i=0;i<nmax;++i)
        {
            Y_ASSERT(A[i]==B[i]);
        }
    }

    template <const size_t BYTES>
    inline void do_bmove(void *a,void *b, const double D)
    {
        init(a,b,BYTES);
        core::bmove<BYTES>(a,b);
        checkMoved(a,BYTES);
        rt_clock chrono;
        uint64_t start  = rt_clock::ticks();
        uint64_t bticks = 0;
        uint64_t mticks = 0;
        unsigned count  = 0;
        while( chrono( rt_clock::ticks() - start ) <= D )
        {
            ++count;
            { const uint64_t mark = rt_clock::ticks(); core::bmove<BYTES>(a,b); bticks += rt_clock::ticks() - mark; }
            { const uint64_t mark = rt_clock::ticks(); memcpy(a,b,BYTES);       mticks += rt_clock::ticks() - mark; }
        }
        const double bspeed = (count/chrono(bticks)) * 1e-6;
        const double mspeed = (count/chrono(mticks)) * 1e-6;
        std::cerr << "<" << BYTES << "> : speed=" << bspeed << "/" << mspeed << std::endl;
        ios::ocstream::echo("bmove.dat","%u %.15g %.15g\n", unsigned(BYTES), bspeed, mspeed);
    }


#define TEST(N) \
do { \
std::cerr << "\t(*) Test " << N << std::endl;\
__bmove<N>(to,fm); \
do_bmove<N>(a,b,D); \
} while(false)

    template <typename T>
    inline void bmove( T &a, const T &b )
    {
        struct mocking { char data[sizeof(T)]; };
        *(reinterpret_cast<mocking *>(&a)) =   *(reinterpret_cast<const mocking *>(&b));
    }

    template <typename T>
    inline void check_bmove()
    {
        std::cerr << "check_bmove.sizeof=" << sizeof(T) << std::endl;
        for(size_t iter=0;iter<1024;++iter)
        {
            T a = support::get<T>();
            T b = support::get<T>();
            bmove(a,b);
            checkMoved(&a,&b,sizeof(T));
        }
    }




}

Y_UTEST(bmove)
{
    double D = 0.1;
    if( argc > 1 ) D = atof( argv[1] );
    int fm[128] = { 4, 7 , 0, 1};
    int to[128];
    int to2[128];

    __bmove<32*sizeof(int)>(to,fm);
    __bmove<32>(to2,fm);

    printf("%d %d %d %d\n",to[0],to[1], to[2], to[3]);

    const size_t nmax=256;
    memory::cblock_of<char> blk(2*nmax);
    void *a = blk.data;
    void *b = blk.data+nmax;

    check_bmove<int8_t>();
    check_bmove<uint8_t>();

    check_bmove<int16_t>();
    check_bmove<uint16_t>();

    check_bmove<int32_t>();
    check_bmove<uint32_t>();

    check_bmove<int64_t>();
    check_bmove<uint64_t>();

    check_bmove<float>();
    check_bmove<double>();

    check_bmove< complex<float>  >();
    check_bmove< complex<double> >();

    check_bmove< point2d<float> >();
    check_bmove< point3d<float> >();
    check_bmove< point2d<double> >();
    check_bmove< point3d<double> >();

    ios::ocstream::overwrite("bmove.dat");
    TEST(1);  TEST(2);  TEST(3); TEST(4); TEST(5); TEST(6); TEST(7); TEST(8); TEST(9);  TEST(10);
    TEST(11); TEST(12); TEST(13); TEST(14); TEST(15); TEST(16); TEST(17); TEST(18); TEST(19);  TEST(20);
    TEST(21); TEST(22); TEST(23); TEST(24); TEST(25); TEST(26); TEST(27); TEST(28); TEST(29);  TEST(30);
    TEST(31); TEST(32);
    TEST(64);
    TEST(80);
    TEST(128);




}
Y_UTEST_DONE()

