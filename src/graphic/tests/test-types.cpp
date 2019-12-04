
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/convert.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include <iomanip>
#include <cstdio>

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

    std::cerr.flush();
    std::cout.flush();
    if(false)
    {
        for(size_t i=0;i<=3*255;++i)
        {
            unsigned g = i/3;
            if(i>0&&g<=0) g=1;
            fprintf(stdout," 0x%02x,",g);
            if(0==((i+1)%16))
            {
                fprintf(stdout, "\n" );
            }
        }
    }

    for(size_t r=0;r<256;++r)
    {
        for(size_t g=0;g<256;++g)
        {
            for(size_t b=0;b<256;++b)
            {
                const uint8_t gs = Convert::GreyScale(r,g,b);
                if(r<=0&&g<=0&&b<=0)
                {
                    Y_ASSERT(gs<=0);
                }
                else
                {
                    Y_ASSERT(gs>0);
                }
            }
        }
    }

}
Y_UTEST_DONE()

