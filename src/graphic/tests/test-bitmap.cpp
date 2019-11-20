
#include "y/graphic/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/type/bzset.hpp"
#include "y/sequence/list.hpp"
#include "y/concurrent/scheme/simd.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(bitmap)
{
    concurrent::simd loop;

    Metrics mtx;
    for(size_t iter=0;iter<1024;++iter)
    {
        bzset(mtx);
        mtx.width  = 1 + alea.leq(100);
        mtx.height = 1 + alea.leq(100);
        mtx.depth  = 1 << ( alea.leq(3));
        mtx.stride = mtx.width * mtx.depth;
        mtx.count  = mtx.stride * mtx.height;
        mtx.entry  = Metrics::Acquire(mtx.count);
        mtx.flags  = Metrics::GlobalMemory | Metrics::ReadWrite;

        Bitmap bmp( mtx );

        bmp.clear();

        list<Bitmap> views;
        std::cerr << "bitmap area: "; bmp.displayArea();
        for(size_t i=0;i<32;++i)
        {
            const Rectangle rect( alea.range<unit_t>(0,bmp.wm), alea.range<unit_t>(0,bmp.hm),
                                 alea.range<unit_t>(0,bmp.wm), alea.range<unit_t>(0,bmp.hm) );
            //std::cerr << "\trect : "; rect.displayRectangle();
            //std::cerr << "\tarea : "; rect.displayArea();
            Y_ASSERT(bmp.contains(rect));
            {
                const Bitmap sub( bmp, rect );
                views.push_back(sub);
            }
            views.back().clear();
        }

        bmp.clear(loop);
    }

    


}
Y_UTEST_DONE()
