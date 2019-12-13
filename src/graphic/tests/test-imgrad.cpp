
#include "y/graphic/image.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/ops/gradient.hpp"
#include "y/utest/run.hpp"
#include "y/fs/vfs.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"
#include "y/graphic/color/ramp/greyscale.hpp"

using namespace upsylon;
using namespace Graphic;



Y_UTEST(imgrad)
{
    Image              &IMG  = Image::instance();
    ForEach             par  = new concurrent::simd();
    const Ramp::Pointer ramp = new Greyscale();
    ColorRamp<float>    proc(ramp);


    for(int i=1;i<argc;++i)
    {
        const string filename = argv[i];
        string       saveName = vfs::get_base_name(filename);
        string       dataName = saveName;
        vfs::change_extension(dataName, "data.png");
        vfs::change_extension(saveName, "grad.png");


        Pixmap3  pxm( IMG.loadAs<rgb>(filename) );
        Tiles    tiles( *pxm, par );
        Gradient grad(pxm->w,pxm->h);

        // convert to float
        Ops::Run(tiles, grad.field, pxm, Convert::Get<float,rgb> );

        // gradient
        grad.compute(tiles);

        // save
        IMG.saveAs(dataName,grad.field,NULL);
        IMG.save(saveName,*grad, proc, NULL);

    }

}
Y_UTEST_DONE()

