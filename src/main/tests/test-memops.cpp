#include "y/type/memops.hpp"
#include "y/utest/run.hpp"
#include "y/os/wtime.hpp"
#include "y/type/spec.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"

#include <iomanip>

using namespace upsylon;

namespace {


#define Y_REP(MACRO) \
MACRO(1); MACRO(2); MACRO(3); MACRO(4); MACRO(5); MACRO(6); MACRO(7); MACRO(8); MACRO(9); \
MACRO(10);MACRO(11);MACRO(12); MACRO(13); MACRO(14); MACRO(15); MACRO(16); MACRO(17); MACRO(18); MACRO(19);\
MACRO(20);MACRO(21);MACRO(22); MACRO(23); MACRO(24); MACRO(25); MACRO(26); MACRO(27); MACRO(28); MACRO(29);\
MACRO(30);MACRO(31);MACRO(32); MACRO(40); MACRO(96); MACRO(128)

    static const size_t ILOG_MAX = ilog2<sizeof(uint64_t)>::value;

    template <size_t N>
    static inline void testType()
    {
        typedef typename core::memcull<N,ILOG_MAX>::word_type word_t;
        std::cerr << "for " << std::setw(3) << N << ": " << type_name_of<word_t>() << std::endl;
    }

    static inline void testTypes()
    {
#define TT(I) testType<I>()

        std::cerr << "-- testing types" << std::endl;
        Y_REP(TT);
    }


    static inline bool isZero_(const void *addr, const size_t size) throw()
    {
        const uint8_t *p = static_cast<const uint8_t *>(addr);
        for(size_t i=0;i<size;++i)
        {
            if(p[i]!=0) return false;
        }
        return true;
    }

    template <typename T>
    static inline bool isZero( const T &tmp ) throw()
    {
        return isZero_( &tmp, sizeof(tmp) );
    }

    template <typename T,const size_t N>
    void testZeroBlock()
    {

        T tmp[N];
        alea.fill(tmp,sizeof(tmp));
        while( isZero_(tmp,sizeof(tmp)) ) alea.fill(tmp,sizeof(tmp));

        for(size_t i=0;i<N;++i)
        {
            memops::zero(tmp[i]);
        }

        for(size_t i=0;i<N;++i)
        {
            Y_ASSERT( isZero(tmp[i]) );
        }
    }



    template <typename T> static inline
    void testZero()
    {
        typedef core::memops<sizeof(T)> ops;
        typedef typename ops::word_type word_type;
        std::cerr << "zero <" << type_name_of<T>() << ">, size=" << sizeof(T) << " -> " << type_name_of<word_type>() << "/" << ops::num_words << std::endl;
        {
            T tmp;
            alea.fill( &tmp, sizeof(tmp) );
            while(isZero(tmp)) alea.fill(&tmp,sizeof(tmp));
            memops::zero(tmp);
            Y_ASSERT(isZero(tmp));
        }
#define TZB(I) testZeroBlock<T,I>()
        Y_REP(TZB);

    }

    static inline void testZeros()
    {
        std::cerr << "-- testing zeros" << std::endl;
        testZero<char>();
        testZero<int16_t>();
        testZero<int32_t>();
        testZero<int64_t>();
        testZero<double>();
        testZero<float>();
        testZero< complex<float>  >();
        testZero< complex<double> >();
        testZero< point2d<char>   >();
        testZero< point3d<char>   >();
        testZero< point3d<int16_t> >();
    }

    template <size_t N>
    static inline void testPerfZero( )
    {
        typedef core::memops<N>         ops;
        typedef typename ops::word_type word_type;
        char *tab = (char *)malloc( 1024 );
        if(!tab) throw exception("no memory for chars");

        uint64_t s = 0;
        uint64_t y = 0;
        wtime    chrono;
        size_t   cycle=0;
        while(chrono(s)<=0.1)
        {
            ++cycle;
            {
                alea.fill(tab,N);
                const uint64_t mark = rt_clock::ticks();
                memset(tab,0,N);
                s += rt_clock::ticks() - mark;
                Y_ASSERT(isZero_(tab,N));
            }

            {
                alea.fill(tab,N);
                const uint64_t mark = rt_clock::ticks();
                core::memrun<word_type,ops::num_words>::zero(tab);
                y += rt_clock::ticks() - mark;
                Y_ASSERT(isZero_(tab,N));
            }
        }
        free(tab);

        const double bytes = N * cycle;
        const double sRate = 1e-6*bytes/chrono(s);
        const double yRate = 1e-6*bytes/chrono(y);
        std::cerr << N << " | s=" << std::setw(10) << sRate << " | y=" <<  std::setw(10) << yRate << std::endl;

    }

    static inline void testPerfZeros( )
    {
        std::cerr << "-- testing perf/zero" << std::endl;
#define TPZ(I) testPerfZero<I>()
        Y_REP(TPZ);
    }

    template <typename T, size_t N> static inline
    void testCopyBlock(const bool doCheck)
    {
        T src[N]; alea.fill(src,sizeof(src));
        T tgt[N]; memset(tgt,0,sizeof(tgt));
        for(size_t i=0;i<N;++i)
        {
            memops::copy(tgt[i], src[i]);
            assert( 0 == memcmp(&tgt[i], &src[i], sizeof(T) ) );
        }

        if(doCheck)
        {
            for(size_t i=0;i<N;++i)
            {
                Y_ASSERT( tgt[i] == src[i] );
            }
        }

    }


    template <typename T> static inline
    void testCopy(const bool doCheck=true)
    {
        typedef core::memops<sizeof(T)> ops;
        typedef typename ops::word_type word_type;
        std::cerr << "copy <" << type_name_of<T>() << ">, size=" << sizeof(T) << " -> " << type_name_of<word_type>() << "/" << ops::num_words << std::endl;
        {
            T       src; alea.fill(&src,sizeof(T));
            T       tgt; memset(&tgt,0,sizeof(T));  Y_ASSERT(isZero(tgt));
            memops::copy(tgt,src);
            Y_ASSERT(0 == memcmp(&src, &tgt, sizeof(T) ) );
        }
#define TCB(I) testCopyBlock<T,I>(doCheck)
        Y_REP(TCB);
    }

    static inline void testCopies()
    {
        std::cerr << "-- testing copies" << std::endl;
        testCopy<char>();
        testCopy<int16_t>();
        testCopy<int32_t>();
        testCopy<int64_t>();
        testCopy<double>(false);
        testCopy<float>(false);
        //testCopy< complex<float>  >();
        //testCopy< complex<double> >();
        testCopy< point2d<char>   >();
        testCopy< point3d<char>   >();
        testCopy< point3d<int16_t> >();
    }

    template <size_t N>
    static inline void testPerfCopy( )
    {
        typedef core::memops<N>         ops;
        typedef typename ops::word_type word_type;
        char *src = (char *)malloc( 1024 );
        char *tgt = src + 512;
        if(!src) throw exception("no memory for chars");

        uint64_t s = 0;
        uint64_t y = 0;
        wtime    chrono;
        size_t   cycle=0;
        while(chrono(s)<=0.1)
        {
            ++cycle;
            {
                alea.fill(src,N);
                memset(tgt,0,N);
                const uint64_t mark = rt_clock::ticks();
                memcpy(tgt,src,N);
                s += rt_clock::ticks() - mark;
                Y_ASSERT( 0 == memcmp(tgt,src,N) );
            }

            {
                alea.fill(src,N);
                memset(tgt,0,N);
                const uint64_t mark = rt_clock::ticks();
                core::memrun<word_type,ops::num_words>::copy(tgt,src);
                y += rt_clock::ticks() - mark;
                Y_ASSERT( 0 == memcmp(tgt,src,N) );
            }
        }
        free(src);

        const double bytes = N * cycle;
        const double sRate = 1e-6*bytes/chrono(s);
        const double yRate = 1e-6*bytes/chrono(y);
        std::cerr << N << " | s=" << std::setw(10) << sRate << " | y=" <<  std::setw(10) << yRate << std::endl;

    }

    static inline void testPerfCopy( )
    {
        std::cerr << "-- testing perf/copy" << std::endl;
#define TPC(I) testPerfCopy<I>()
        Y_REP(TPC);
    }

}

Y_UTEST(memops)
{
    testTypes();
    testZeros();
    testCopies();
    testPerfCopy();
    if(false)
    {
        testPerfZeros( );
    }
}
Y_UTEST_DONE()

