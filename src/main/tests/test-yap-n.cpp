#include "y/yap/number.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace yap;

Y_UTEST(yap_n)
{
    std::cerr << "core_bits=" << number::core_bits << std::endl;
    std::cerr << "word_bits=" << number::word_bits << std::endl;
    Y_CHECK(number::word_bits<=number::core_bits/2);
    Y_CHECK((1<<number::word_exp2)==number::word_size);
}
Y_UTEST_DONE()

