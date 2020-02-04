#include "y/type/divs.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

Y_UTEST(divs)
{
    std::cerr << type_spec_of<int>() << std::endl;
    std::cerr << type_spec_of<long>() << std::endl;
    std::cerr << type_spec_of<long long>() << std::endl;

}
Y_UTEST_DONE()

