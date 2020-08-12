#include "y/sibyl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;
using namespace sibyl;

#define SHOW(X) std::cerr << #X << " = " << X << std::endl

namespace {


    template <size_t BITS>
    static inline void doTest()
    {
        typedef natural<BITS> Unsigned;

        std::cerr << "BITS=" << BITS << std::endl;
        SHOW(natural<BITS>::word_size);
        SHOW(natural<BITS>::core_size);
        SHOW(natural<BITS>::word_bits);
        SHOW(natural<BITS>::word_exp2);
        SHOW(natural<BITS>::word_mask);
        SHOW(natural<BITS>::max_word);


        Unsigned z;
        Unsigned z1(200,as_capacity);

        z1.xch(z);
        z1.xch(z);

        std::cerr << "z.count=" << z.count << "/width=" << z.width << std::endl;
        std::cerr << "zero = " << z << std::endl;

        Unsigned one(1);
        std::cerr << "one  = " << one << std::endl;

        Unsigned abcd( 0x0a0b0c0d );
        std::cerr << "abcd = " << abcd << std::endl;

        Unsigned temp(abcd);
        std::cerr << "temp = " << temp << std::endl;



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

