#include "y/mkl/simplify.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace mkl;

Y_UTEST(simplify)
{

    vector<int> a;
    a << -10 << 20 << 0 << 45;
    std::cerr << "a=" << a << std::endl;

    const int r = simplify<int>::on(a);
    std::cerr << "r=" << r << std::endl;
    std::cerr << "a=" << a << std::endl;

}
Y_UTEST_DONE()

