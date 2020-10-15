#include "y/type/block/zset.hpp"
#include "y/utest/run.hpp"

#include <typeinfo>
#include <iomanip>
#include <cstring>

using namespace upsylon;

#define Y_REP(MACRO) \
MACRO(1); MACRO(2); MACRO(3); MACRO(4); MACRO(5); MACRO(6); MACRO(7); MACRO(8); MACRO(9); \
MACRO(10);MACRO(11);MACRO(12); MACRO(13); MACRO(14); MACRO(15); MACRO(16); MACRO(17); MACRO(18); MACRO(19);\
MACRO(20);MACRO(21);MACRO(22); MACRO(23); MACRO(24); MACRO(25); MACRO(26); MACRO(27); MACRO(28); MACRO(29);\
MACRO(30);MACRO(31);MACRO(32); MACRO(40); MACRO(96); MACRO(128)

namespace {


    static const size_t ILOG_MAX = ilog2<sizeof(uint64_t)>::value;

    template <size_t N>
    static inline void testType()
    {
        typedef typename core::cull_on<ILOG_MAX,N>::word_type word_t;
        std::cerr << "for " << std::setw(3) << N << ": uint" << sizeof(word_t)*8 << "_t \t'" << typeid(word_t).name() << "'" << std::endl;
        typedef cull<N> culled;
        Y_ASSERT( culled::num_words * culled::word_size == N );
    }

    static inline void testTypes()
    {
        std::cerr << "-- testing types" << std::endl;
#define _testType(I) testType<I>()
        Y_REP(_testType);
    }

}

#include "y/type/point3d.hpp"

#define Y_WITH(CALL) \
CALL<int8_t>(); \
CALL<int16_t>();\
CALL<int32_t>();\
CALL<int64_t>();\
CALL<double>(false);\
CALL<float>(false);\
CALL< complex<float>   >( false );\
CALL< complex<double>  >( false );\
CALL< point2d<char>    >();\
CALL< point3d<int16_t> >();\
CALL< point3d<int64_t> >()


namespace {

    template <size_t N>
    static inline void testZero()
    {
        void *addr = calloc(1,N);
        if(!addr) throw exception("no memory in testZero");

        alea.fillnz(addr,N);
        block_zset<N>(addr);
        Y_ASSERT( block_is_zeroed<N>(addr) );
        free(addr);

    }


    template <typename T>
    static inline void zeroType(const bool checkZ = true )
    {
        std::cerr << " |_<" << typeid(T).name() << ">" << std::endl;
        std::cerr << "  |_[";
        for(size_t iter=64;iter>0;--iter)
        {
            T tmp;
            alea.fillnz(&tmp,sizeof(T)); bzset(tmp);                   Y_ASSERT(is_zeroed(tmp) || die("bzset") );
            alea.fillnz(&tmp,sizeof(T)); block_zset<sizeof(T)>(&tmp);  Y_ASSERT(is_zeroed(tmp) || die("block_zset") );
            if(checkZ)
            {
                const T z = 0;
                Y_ASSERT( z == tmp );
                std::cerr << "+";
            }
            else
            {
                std::cerr << ".";

            }
        }
        std::cerr << "]" << std::endl;
    }

    static inline void zeroTypes()
    {
        Y_WITH(zeroType);
    }



    static inline void testZeros()
    {
        std::cerr << "-- testing zeros" << std::endl;
#define _testZero(I) testZero<I>()
        Y_REP(_testZero);
        zeroTypes();
        std::cerr << std::endl;
    }
}

#include "y/type/block/move.hpp"

namespace {

    template <size_t N>
    static inline void testMove()
    {
        char *target = (char *)calloc(2,N);
        char *source = target+N;
        if(!target) throw exception("no memory in testMove");

        alea.fillnz(source,N);
        block_zset<N>(target);        Y_ASSERT(block_is_zeroed<N>(target));
        block_move<N>(target,source); Y_ASSERT( 0 == memcmp(target,source,N) );
        free(target);
    }

    template <typename T>
    static inline void moveType(const bool check=true)
    {
        std::cerr << " |_<" << typeid(T).name() << "> / size=" << sizeof(T) << std::endl;
        std::cerr << "  |_[";
        for(size_t iter=64;iter>0;--iter)
        {
            T target;
            T source;
            alea.fillnz(&source,sizeof(T));
            bzset(target);        Y_ASSERT( is_zeroed(target) );
            bmove(target,source); Y_ASSERT( 0 == memcmp( &target, &source, sizeof(T) ) );

            if(check)
            {
                Y_ASSERT( target == source );
                std::cerr << "+";
            }
            else
            {
                std::cerr << ".";

            }
        }
        std::cerr << "]" << std::endl;
    }

    static inline void moveTypes()
    {
        Y_WITH(moveType);
    }

    static inline void testMoves()
    {
        std::cerr << "-- testing Moves" << std::endl;
#define _testMove(I) testMove<I>()
        Y_REP(_testMove);
        moveTypes();
        std::cerr << std::endl;
    }
}

#include "y/type/block/swap.hpp"

namespace {

    template <size_t N>
    static inline void testSwap()
    {
        char *target = (char *)calloc(3,N);
        char *source = target+N;
        char *origin = source+N;
        if(!target) throw exception("no memory in testSwap");

        alea.fillnz(source,N);
        block_zset<N>(target);        Y_ASSERT( block_is_zeroed<N>(target)   );
        block_move<N>(origin,source); Y_ASSERT( 0 == memcmp(origin,source,N) );
        block_swap<N>(target,source);
        Y_ASSERT( block_is_zeroed<N>(source)   );
        Y_ASSERT( 0 == memcmp(target,origin,N) );
        free(target);
    }

    template <typename T> static inline
    void swapType(const bool check=true)
    {
        std::cerr << " |_<" << typeid(T).name() << "> / size=" << sizeof(T) << std::endl;
        std::cerr << "  |_[";
        for(size_t iter=64;iter>0;--iter)
        {
            T target;
            T source;
            T origin;
            alea.fillnz(&source,sizeof(T)); Y_ASSERT( !is_zeroed(source) );
            bzset(target);        Y_ASSERT( is_zeroed(target)    );
            bmove(origin,source); Y_ASSERT( bsame(origin,source) );
            bswap(target,source); Y_ASSERT( bsame(target,origin) );
            std::cerr << "source=" << source << std::endl;
            Y_ASSERT( is_zeroed(source) );
            if(check)
            {
                Y_ASSERT( target == origin );
                std::cerr << "+";
            }
            else
            {
                std::cerr << ".";

            }
        }
        std::cerr << "]" << std::endl;
    }


    static inline void swapTypes()
    {
        Y_WITH(swapType);
    }

    static inline void testSwaps()
    {
        std::cerr << "-- testing Swaps" << std::endl;
#define _testSwap(I) testSwap<I>()
        Y_REP(_testSwap);
        swapTypes();
        std::cerr << std::endl;
    }
}

Y_UTEST(cull)
{
    testTypes();
    testZeros();
    testMoves();
    testSwaps();
}
Y_UTEST_DONE()

#include "y/os/wtime.hpp"
#include "y/code/hr-ints.hpp"

namespace {

    template <size_t N>
    static inline void testZSET()
    {
        std::cerr << "N=" << std::setw(3) << N;
        void *addr = calloc(1,N);
        if(!addr) throw exception("no memory in testZSET");
        void *wksp = aliasing::anonymous(addr);

        uint64_t s = 0;
        uint64_t y = 0;
        uint64_t count = 0;
        wtime chrono;

        while( chrono(s) <= 0.1 )
        {
            count += N;
            {
                const uint64_t mark = chrono.ticks();
                memset(wksp,0,N);
                s += chrono.ticks() - mark;
            }

            {
                const uint64_t mark = chrono.ticks();
                block_zset<N>(wksp);
                y += chrono.ticks() - mark;
            }

        }

        const human_readable sRate = int64_t( floor( double(count)/chrono(s) + 0.5 ) );
        const human_readable yRate = int64_t( floor( double(count)/chrono(y) + 0.5 ) );

        std::cerr << " | sRate=" << sRate << " | yRate=" << yRate << std::endl;

        free(addr);
    }



}

Y_UTEST(zset_perf)
{
    
#define _testZSET(I) testZSET<I>()
    Y_REP(_testZSET);

}
Y_UTEST_DONE()

namespace {

    template <size_t N>
    static inline void testMOVE()
    {
        std::cerr << "N=" << std::setw(3) << N;
        void *addr = calloc(1,2*N);
        if(!addr) throw exception("no memory in testMOVE");
        char *target = aliasing::as<char>(addr);
        char *source = target + N;

        uint64_t c = 0;
        uint64_t m = 0;
        uint64_t y = 0;
        uint64_t count = 0;
        wtime chrono;

        while( chrono(c) <= 0.1 )
        {
            count += N;
            {
                const uint64_t mark = chrono.ticks();
                memcpy(target,source,N);
                c += chrono.ticks() - mark;
            }

            {
                const uint64_t mark = chrono.ticks();
                memmove(target,source,N);
                m += chrono.ticks() - mark;
            }

            {
                const uint64_t mark = chrono.ticks();
                block_move<N>(target,source);
                y += chrono.ticks() - mark;
            }

        }

        const human_readable cRate = int64_t( floor( double(count)/chrono(c) + 0.5 ) );
        const human_readable mRate = int64_t( floor( double(count)/chrono(m) + 0.5 ) );
        const human_readable yRate = int64_t( floor( double(count)/chrono(y) + 0.5 ) );

        std::cerr << " | cRate=" << cRate << " | mRate=" << mRate << " | yRate=" << yRate << std::endl;

        free(addr);
    }



}

Y_UTEST(move_perf)
{

#define _testMOVE(I) testMOVE<I>()
    Y_REP(_testMOVE);

}
Y_UTEST_DONE()

