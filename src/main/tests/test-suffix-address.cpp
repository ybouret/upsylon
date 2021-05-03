#include "y/associative/suffix/address.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(suffix_address)
{
    const short a = 10;
    const float f = 2.0f;

    suffix_address aa(a);
    suffix_address af(f);

    std::cerr << "aa: "; aa.print_to(std::cerr) << std::endl;
    std::cerr << "af: "; af.print_to(std::cerr) << std::endl;

    Y_CHECK( aa.addr() == &a );
    Y_CHECK( af.addr() == &f );

    std::cerr << "a=" << aa.to<short>() << std::endl;
    std::cerr << "f=" << af.to<float>() << std::endl;

}
Y_UTEST_DONE()

