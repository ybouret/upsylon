#include "y/type/binary.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(binary)
{
    float f = 2;
    float g = 2;

    std::cerr << "f=" << binary(f) << std::endl;
    std::cerr << "g=" << binary(g) << std::endl;



    double F = 1.0;
    double G = 1.0;
    std::cerr << binary(F) << std::endl;
    std::cerr << binary(G) << std::endl;
    Y_CHECK(f==g);
    Y_CHECK(F==G);



}
Y_UTEST_DONE()

