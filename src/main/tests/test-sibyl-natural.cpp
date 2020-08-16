#include "y/sibyl/natural.hpp"
#include "y/sibyl/types.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/code/utils.hpp"
#include "y/ios/ocstream.hpp"
#include "y/code/hr-ints.hpp"

using namespace upsylon;
using namespace sibyl;


#define SHOW(X) std::cerr << "\t" << #X << " = " << int64_t(X) << std::endl

namespace {
    
    static const char add_fn[] = "mpn-add-ticks.dat";
    
    template <size_t BITS>
    static inline void doTest()
    {
        rt_clock clk;
        
        std::cerr << "<natural BITS=" << BITS << ">" << std::endl;
        std::cerr << std::dec;
        typedef natural<BITS> Unsigned;
        std::cerr << "\t<parameters>" << std::endl;
        SHOW(natural<BITS>::word_bits);
        SHOW(natural<BITS>::word_size);
        SHOW(natural<BITS>::core_size);
        SHOW(natural<BITS>::word_bits);
        SHOW(natural<BITS>::word_exp2);
        SHOW(natural<BITS>::word_mask);
        SHOW(natural<BITS>::max_word);
        SHOW(natural<BITS>::words_per_utype);
        std::cerr << "\t<parameters/>" << std::endl;
        
        
        std::cerr << "-- initializing with zero or bits" << std::endl;
        {
            Unsigned zero;
            std::cerr << "zero=" << zero << std::endl;
            Y_CHECK( zero.lsw() == 0 );
            {
                Unsigned tmp(100,as_capacity);
                tmp.xch(zero);
                zero.xch(tmp);
                Y_CHECK(zero==tmp); Y_CHECK( !(zero!=tmp) );
            }
            Y_CHECK(zero==0);
            Y_CHECK(0==zero);
        }
        
        {
            Unsigned x = 0; Y_ASSERT( x.lsw() == 0 );
            for(size_t i=0;i<1024;++i)
            {
                const size_t bits = alea.leq(64);
                x = Unsigned(alea,bits);    Y_ASSERT(x.bits()==bits);
                const uint64_t u = x.lsw(); Y_ASSERT(bits_for(u)==bits);
                for(size_t i=0;i<bits;++i)
                {
                    const uint64_t b = uint64_t(1)<<i;
                    const bool     f = (0 != (u&b));
                    Y_ASSERT( f == x.get_bit(i) );
                }
                const Unsigned y = u;
                Y_ASSERT( x == y ); Y_ASSERT( ! (x!=y) );
                Y_ASSERT( x == u ); Y_ASSERT( ! (x!=u) );
                Y_ASSERT( u == x ); Y_ASSERT( ! (u!=x) );
                
            }
        }
        
        std::cerr << "-- small additions" << std::endl;
        for(size_t iter=0;iter<1024*16;++iter)
        {
            const uint64_t a = alea.partial<uint64_t>( alea.leq(63) );
            const uint64_t b = alea.partial<uint64_t>( alea.leq(63) );
            const uint64_t c = a+b; Y_ASSERT(c>=a); Y_ASSERT(c>=b);
            const Unsigned A = a;   Y_ASSERT( A.lsw() == a );
            const Unsigned B = b;   Y_ASSERT( B.lsw() == b );
            const Unsigned C = A+B; Y_ASSERT( C.lsw() == c );
        }
        
        std::cerr << "-- all additions" << std::endl;
        ios::ocstream::echo(add_fn, "%2d", int(BITS));
        for(size_t bits=1;bits<=4096;bits <<=1)
        {
            Unsigned::add_ticks = 0;
            static const size_t add_cycles = 1024*32;
            for(size_t iter=0;iter<add_cycles;++iter)
            {
                const Unsigned A(alea,alea.range<size_t>(bits-0,bits));
                const Unsigned B(alea,alea.range<size_t>(bits-0,bits));
                Unsigned C = A+B;
                C.ldz();
            }
            const double add_time = clk(Unsigned::add_ticks);
            const double speed    = add_cycles/(add_time*1e6);
            ios::ocstream::echo(add_fn," %8.2lf",speed);
        }
        ios::ocstream::echo(add_fn,"\n");
        
        
        
        std::cerr << "<natural/>" << std::endl;
        std::cerr << std::endl;
        
        
    }
}

union wrap8 {
    uint8_t w;
    struct {
        uint8_t l: 4;
        uint8_t h: 4;
    } r;
};

Y_UTEST(sibyl_n)
{
    if(false)
    {
        ios::ocstream::overwrite(add_fn);
        ios::ocstream::echo(add_fn, "#b");
        for(unsigned bits=1;bits<=4096;bits <<=1)
        {
            ios::ocstream::echo(add_fn," %8u", bits);
        }
        ios::ocstream::echo(add_fn,"\n");
        
        SHOW(number::sys_core_size); std::cerr << type_name_of<number::sys_core_type>() << std::endl;
        SHOW(number::max_core_size); std::cerr << type_name_of<number::max_core_type>() << std::endl;
        SHOW(number::max_word_size);
        
        doTest<8>();
        doTest<16>();
        doTest<32>();
        
        std::cerr << *number::instance() << std::endl;
    }
    
    const uint8_t x = 0xab;
    wrap8         y = { x };
    std::cerr << "sizeof(wrap8)=" << sizeof(wrap8) << std::endl;
    std::cerr << std::hex;
    std::cerr << "x=" << int(x) << ", y.w=" << int(y.w) << ", l=" << int(y.r.l) << ", h=" << int(y.r.h) << std::endl;
    
    
}
Y_UTEST_DONE()

