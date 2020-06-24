#include "y/type/binary.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(binary)
{
    binary<float> f = 2;
    binary<float> g = 2;

    std::cerr << "f=" << f << std::endl;
    std::cerr << "g=" << g << std::endl;

    Y_CHECK(f==g);

    double F = 1.0;
    double G = 1.0;
    std::cerr << binary<double>(F) << std::endl;
    std::cerr << binary<double>(G) << std::endl;


}
Y_UTEST_DONE()

