#include "y/graphic/ops/3x3.hpp"
#include "y/utest/run.hpp"
 #include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"

using namespace upsylon;
using namespace Graphic;


template <typename T>
static inline
void apply( const Pixmap<T> &source, Tiles &tiles, const char *sfx )
{
    Image              &IMG      = Image::instance();

    Pixmap<T> target( source->w, source->h );

    Apply3x3::Median(target, source, tiles);
    {
        string saveName = "median";
        saveName += sfx;
        saveName += ".png";
        IMG.saveAs(saveName, target, 0);
    }

    Apply3x3::Average(target, source, tiles);
    {
        string saveName = "average";
        saveName += sfx;
        saveName += ".png";
        IMG.saveAs(saveName, target, 0);
    }

    Apply3x3::Erode(target, source, tiles);
    {
        string saveName = "erode";
        saveName += sfx;
        saveName += ".png";
        IMG.saveAs(saveName, target, 0);
    }


    Apply3x3::Dilate(target, source, tiles);
    {
        string saveName = "dilate";
        saveName += sfx;
        saveName += ".png";
        IMG.saveAs(saveName, target, 0);
    }

    Pixmap<T> tmp( source->w, source->h );
    Apply3x3::Open(target, source, tiles, tmp);
    {
        string saveName = "open";
        saveName += sfx;
        saveName += ".png";
        IMG.saveAs(saveName, target, 0);
    }

    Apply3x3::Close(target, source, tiles, tmp);
    {
        string saveName = "close";
        saveName += sfx;
        saveName += ".png";
        IMG.saveAs(saveName, target, 0);
    }



}

Y_UTEST(3x3)
{
    ForEach             par = new concurrent::simd();

    if(argc>1)
    {
        const string        filename = argv[1];
        Image              &IMG      = Image::instance();
        const Pixmap<uint8_t> pxm1( IMG.loadAs<uint8_t>(filename) );
        const Pixmap<float>   pxmf(  IMG.loadAs<float>(filename) );
        const Pixmap<rgb>     pxm3( IMG.loadAs<rgb>(filename)  );
        const Pixmap<rgba>    pxm4( IMG.loadAs<rgba>(filename)  );

        IMG.saveAs("org.png", pxm4, 0);
        Tiles     tiles( *pxm1, par );

        apply( pxm1, tiles, "1" );
        apply( pxmf, tiles, "f" );
        apply( pxm3, tiles, "3" );
        apply( pxm4, tiles, "4" );


    }

}
Y_UTEST_DONE()
