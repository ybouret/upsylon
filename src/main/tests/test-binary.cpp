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
    
}
Y_UTEST_DONE()

