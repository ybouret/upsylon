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

        class broker : tiles, concurrent::runnable
        {
        public:
            const engine  loop;

            explicit broker(const area &a, const engine &l) :
            tiles(a,*l),
            loop(l)
            {
                assert(  size() <= loop->size() );
            }

            typedef void (*func)(const tile &t, void *args, lockable &);

            void operator()(func f, void *p)
            {
                kCode = f;
                kArgs = p;
                aliasing::_(loop)->for_each(*this);
            }

            func  kCode;
            void *kArgs;

            virtual void run(const concurrent::context &ctx, lockable &sync) throw()
            {
                assert(kCode);
                assert(kArgs);
                const size_t rank = ctx.rank;

                if(rank<size())
                {
                    kCode( (*this)[rank], kArgs, sync);
                }
            }

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

    std::cerr << "sequential" << std::endl;
    {
        broker seq(pxm,seq_loop);
        //seq();
        std::cerr << std::endl;
    }

    std::cerr << "parallel" << std::endl;
    {
        broker par(pxm,par_loop);
        // par();
    }




}
Y_UTEST_DONE()

