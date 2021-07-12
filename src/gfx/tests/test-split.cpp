#include "y/gfx/image/io.hpp"
#include "y/gfx/ops/split.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/gfx/color/named.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(split)
{
    Y_USE_IMG();
    engine eng = new concurrent::simt();

    if(argc>1)
    {
        const pixmap<rgba> img = IMG.load<rgba>(argv[1]);
        IMG.save(img,"img.png");
        std::cerr << "loaded: " << img << std::endl;
        broker       apply(eng,img);
        
    }

}
Y_UTEST_DONE()

