#include "y/sibyl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;
using namespace sibyl;

#define SHOW(X) std::cerr << #X << " = " << int64_t(X) << std::endl

namespace {


    template <size_t BITS>
    static inline void doTest()
    {
        std::cerr << std::dec;
        typedef natural<BITS> Unsigned;

        std::cerr << "BITS=" << BITS << std::endl;
        SHOW(natural<BITS>::word_bits);
        SHOW(natural<BITS>::word_size);
        SHOW(natural<BITS>::core_size);
        SHOW(natural<BITS>::word_bits);
        SHOW(natural<BITS>::word_exp2);
        SHOW(natural<BITS>::word_mask);
        SHOW(natural<BITS>::max_word);
        SHOW(natural<BITS>::words_per_utype);


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



        std::cerr << std::endl;
    }
}

Y_UTEST(sibyl_n)
{
    SHOW(number::sys_core_size); std::cerr << type_name_of<number::sys_core_type>() << std::endl;
    SHOW(number::max_core_size); std::cerr << type_name_of<number::max_core_type>() << std::endl;
    SHOW(number::max_word_size);

    doTest<8>();
    doTest<16>();
    doTest<32>();
    //doTest<64>();

    std::cerr << *number::instance() << std::endl;

}
Y_UTEST_DONE()

