#include "y/type/mat2x2.hpp"
#include "y/type/mat3x3.hpp"
#include "y/yap.hpp"
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void doTest()
    {
        std::cerr << "for <" << type_name_of<T>() << std::endl;
        mat2x2<T> m2;
        std::cerr << "m2=" << m2 << std::endl;

        mat3x3<T> m3;
        std::cerr << "m3=" << m3 << std::endl;
    }
}

Y_UTEST(geomat)
{
    doTest<int>();
    doTest<float>();
    doTest<double>();
    doTest<apq>();
}
Y_UTEST_DONE()

