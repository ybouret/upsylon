
#include "y/graphic/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(types)
{

    size_t  num = 5;
    std::cerr << "required objects = " << num << std::endl;
    double *arr = Memory::AcquireAs<double>(num);
    std::cerr << "allocated bytes  = " << num << std::endl;
    Memory::ReleaseAs(arr,num);
    Y_CHECK(0==num);
    Y_CHECK(0==arr);

    const char pfx[] = "\t";
    Y_UTEST_SIZEOF_WITH( pfx, Area);
    Y_UTEST_SIZEOF_WITH( pfx, Rectangle);
    Y_UTEST_SIZEOF_WITH( pfx, Bitmap);
    Y_UTEST_SIZEOF_WITH( pfx, AnonymousRow);
    Y_UTEST_SIZEOF_WITH( pfx, Row<uint8_t>);
    Y_UTEST_SIZEOF_WITH( pfx, Row<uint16_t>);
    Y_UTEST_SIZEOF_WITH( pfx, Row<uint32_t>);

}
Y_UTEST_DONE()

