
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/convert.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include <iomanip>

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
    Y_UTEST_SIZEOF_WITH( pfx, Surface);
    Y_UTEST_SIZEOF_WITH( pfx, Pixmap1);
    Y_UTEST_SIZEOF_WITH( pfx, PixmapF);
    Y_UTEST_SIZEOF_WITH( pfx, PixmapD);
    Y_UTEST_SIZEOF_WITH( pfx, Pixmap3);
    Y_UTEST_SIZEOF_WITH( pfx, Pixmap4);
    Y_UTEST_SIZEOF_WITH( pfx, AnonymousRow);
    Y_UTEST_SIZEOF_WITH( pfx, Row<uint8_t>);
    Y_UTEST_SIZEOF_WITH( pfx, Row<uint16_t>);
    Y_UTEST_SIZEOF_WITH( pfx, Row<uint32_t>);

    std::cerr << "Checking Conversion..." << std::endl;
    for(size_t i=0;i<256;++i)
    {
        Y_ASSERT( Convert::Float2Byte( Convert::UnitFloat[i] ) == i );
    }

    if(false)
    {
        for(size_t i=0;i<256;++i)
        {
            std::cout << "_F(" << std::setw(3) << i << "),";
            if(0==((i+1)%16)) std::cout << std::endl;
        }
    }

}
Y_UTEST_DONE()

