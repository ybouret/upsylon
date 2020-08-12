#include "y/sibyl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace sibyl;

#define SHOW(X) std::cerr << #X << " = " << X << std::endl

namespace {


    template <size_t BITS>
    static inline void doTest()
    {
        std::cerr << "BITS=" << BITS << std::endl;
        SHOW(natural<BITS>::word_size);
        SHOW(natural<BITS>::core_size);
        SHOW(natural<BITS>::possible);
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
    doTest<64>();


}
Y_UTEST_DONE()

