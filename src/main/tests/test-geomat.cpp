#include "y/type/mat2x2.hpp"
#include "y/type/mat3x3.hpp"
#include "y/yap.hpp"
#include "y/type/rtti.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{



    template <typename T>
    void doTest()
    {
        std::cerr << "for <" << rtti::name_of<T>() << std::endl;
        mat2x2<T> m2;
        std::cerr << "m2=" << m2 << std::endl;
        m2.ex.x = 1;
        m2.ex.y = 2;
        m2.ey.x = 3;
        m2.ey.y = 4;
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

