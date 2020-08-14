#include "y/sibyl/natural.hpp"
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

        std::cerr << "add_ticks: " << Unsigned::add_ticks << std::endl;

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
        Unsigned::add_ticks = 0;
        for(size_t iter=0;iter<1024*16;++iter)
        {
            const uint64_t a = alea.partial<uint64_t>( alea.leq(63) );
            const uint64_t b = alea.partial<uint64_t>( alea.leq(63) );
            const uint64_t c = a+b; Y_ASSERT(c>=a); Y_ASSERT(c>=b);
            const Unsigned A = a;   Y_ASSERT( A.lsw() == a );
            const Unsigned B = b;   Y_ASSERT( B.lsw() == b );
            const Unsigned C = A+B; Y_ASSERT( C.lsw() == c );
        }

        std::cerr << "-- large additions" << std::endl;
        for(size_t bits=32;bits<=2048;bits <<=1)
        {
            for(size_t iter=0;iter<1024*16;++iter)
            {
                const Unsigned A(alea,alea.range<size_t>(bits-5,bits));
                const Unsigned B(alea,alea.range<size_t>(bits-5,bits));
                const Unsigned C = A+B;
                (void) C;
            }
        }
        std::cerr << " |_add_ticks : " << human_readable(Unsigned::add_ticks) << std::endl;
        {
            const double t = clk(Unsigned::add_ticks);
            std::cerr << " |_add_time  : " << t << std::endl;
            ios::ocstream::echo(add_fn, "%2d %g\n", int(BITS), t*1e6);
        }



        std::cerr << "<natural/>" << std::endl;
        std::cerr << std::endl;

#if 0
        Unsigned z;
        std::cerr << "zero = " << z << std::endl;
        
        {
            Unsigned z1(200,as_capacity);
            z1.xch(z);
            z1.xch(z);
        }


        Unsigned one(1);
        std::cerr << "one  = " << one << std::endl;

        for(size_t i=0;i<16;++i)
        {
            Unsigned abcd( alea.partial<uint64_t>() );
            Unsigned temp(abcd);
            std::cerr << "abcd = " << abcd << "/" << temp << std::endl;
            Y_ASSERT( Unsigned::eq(abcd,temp) );
            const size_t nb=abcd.bits();
            std::cerr << "#bits=" << nb << " : ";
            for(size_t n=nb;n>0;)
            {
                const bool flag = abcd.get_bit(--n);
                std::cerr << (flag?'1':'0');
            }
            std::cerr << std::endl;
        }

        std::cerr << std::hex;
        for(size_t i=0;i<1024;++i)
        {
            const uint32_t a = alea.partial<uint32_t>(30);
            const uint32_t b = alea.partial<uint32_t>(30);
            const uint32_t c = a+b;
            Y_ASSERT(a+b==c);
            const Unsigned A = a;                  Y_ASSERT( A.lsw() == a );
            const Unsigned B = b;                  Y_ASSERT( B.lsw() == b );
            const Unsigned C = Unsigned::add(A,B); Y_ASSERT( C.lsw() == c );


        }

        for(size_t bits=0;bits<=100;++bits)
        {
            Unsigned r(alea,bits);
            std::cerr << "ran=" << r << std::endl;
            Y_ASSERT(r.bits()==bits);
        }

        std::cerr << std::endl;
#endif
    }
}

Y_UTEST(sibyl_n)
{
    ios::ocstream::overwrite(add_fn);
    SHOW(number::sys_core_size); std::cerr << type_name_of<number::sys_core_type>() << std::endl;
    SHOW(number::max_core_size); std::cerr << type_name_of<number::max_core_type>() << std::endl;
    SHOW(number::max_word_size);

    doTest<8>();
    doTest<16>();
    doTest<32>();

    std::cerr << *number::instance() << std::endl;

}
Y_UTEST_DONE()

