
#include "y/graphic/surface.hpp"
#include "y/utest/run.hpp"
#include "y/hashing/sha1.hpp"
#include "y/sequence/list.hpp"

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
                uint8_t      *p = (uint8_t *) bmp.standardPixel(i,j);
                for(size_t k=0;k<bmp.depth;++k)
                {
                    const uint8_t b = alea.full<uint8_t>();
                    p[k] = b;
                    H( &b, 1);
                }
            }
        }
        digest d0 = H.md();

        H.set();
        std::cerr << "\td0=" << d0 << std::endl;
        for(unit_t j=0;j<bmp.h;++j)
        {
            for(unit_t i=0;i<bmp.w;++i)
            {
                const uint8_t *p = (uint8_t *) bmp.standardPixel(i,j);
                for(size_t k=0;k<bmp.depth;++k)
                {
                    H( &p[k], 1 );
                }
            }
        }
        digest d1 = H.md();
        std::cerr << "\td1=" << d1 << std::endl;
        H.set();
        bmp.run(H);
        digest d2 = H.md();
        std::cerr << "\td2=" << d2 << std::endl;


        std::cerr << "\t"; Y_CHECK(d0==d1);
        std::cerr << "\t"; Y_CHECK(d0==d2);



    }

    static inline Rectangle makeRect(const Area &area)
    {
        return Rectangle(alea.range<unit_t>(0,area.w-1),
                         alea.range<unit_t>(0,area.h-1),
                         alea.range<unit_t>(0,area.w-1),
                         alea.range<unit_t>(0,area.h-1));
    }

    static inline void doGrow(sequence<Surface> &surfaces,
                              const Surface      &S)
    {
        hashing::sha1 H;
        H.set();
        S->run(H);
        const digest org = H.md();
        {
            const Rectangle rect = makeRect( *S );
            Surface         sub  = Bitmap::Share(*S,rect);
            surfaces.push_back(sub);
        }

        {
            Surface sub = Bitmap::Share(*S);
            surfaces.push_back(sub);
            H.set();
            surfaces.back()->run(H);
            const digest shs = H.md();
            Y_ASSERT(org==shs);
        }


        {
            const Rectangle rect = makeRect( *S );
            Surface         sub  = Bitmap::Clone(*S,rect);
            surfaces.push_back(sub);
        }

        {
            Surface sub = Bitmap::Clone(*S);
            surfaces.push_back(sub);
            H.set();
            surfaces.back()->run(H);
            const digest shs = H.md();
            Y_ASSERT(org==shs);
        }
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

    list<Surface> surfaces;
    for(size_t k=0;k<16;++k)
    {
        doGrow(surfaces,bmp1);
        doGrow(surfaces,bmp2);
        doGrow(surfaces,bmp3);
        doGrow(surfaces,bmp4);

    }

    std::cerr << "#sub surfaces=" << surfaces.size() << std::endl;



}
Y_UTEST_DONE()

