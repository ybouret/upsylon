
#include "y/graphic/surface.hpp"
#include "y/utest/run.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;
using namespace Graphic;



#define ALEA_COORD (1+alea.leq(200))

namespace {

    static inline void fill( Bitmap &bmp )
    {
        std::cerr << "BytesPerPixel=" << bmp.depth << std::endl;
        bmp.displayArea();
        hashing::sha1 H;

        H.set();
        for(unit_t j=0;j<bmp.h;++j)
        {
            for(unit_t i=0;i<bmp.w;++i)
            {
                const uint8_t b = alea.full<uint8_t>();
                *(uint8_t*)(bmp.get(i,j)) = b;
                H( &b, 1);
            }
        }
        digest d0 = H.md();

        H.set();
        std::cerr << "\td0=" << d0 << std::endl;
        for(unit_t j=0;j<bmp.h;++j)
        {
            for(unit_t i=0;i<bmp.w;++i)
            {
                H( bmp.get(i,j), 1 );
            }
        }
        digest d1 = H.md();
        std::cerr << "\td1=" << d1 << std::endl;
        std::cerr << "\t"; Y_CHECK(d0==d1);
    }
}

Y_UTEST(bitmap)
{

    Surface  bmp1 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 1);
    Surface  bmp2 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 2);
    Surface  bmp3 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 3);
    Surface  bmp4 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 4);

    fill( *bmp1 );
    fill( *bmp2 );
    fill( *bmp3 );
    fill( *bmp4 );


}
Y_UTEST_DONE()

