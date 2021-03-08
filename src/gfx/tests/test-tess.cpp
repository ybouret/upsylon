#include "y/gfx/area/tiles.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/pixmaps.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

namespace upsylon
{
    namespace graphic
    {

        typedef arc_ptr<concurrent::looper> engine;


        class broker : public concurrent::runnable
        {
        public:
            engine        loop;
            const tiles   tess;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(broker);

        };


    }
}

Y_UTEST(tess)
{

    engine seq_loop = new concurrent::solo();
    engine par_loop = new concurrent::simt();

    pixmap<float> pxm(100,50);


}
Y_UTEST_DONE()

