#include "y/gfx/async/outer-worker.hpp"

namespace upsylon
{
    namespace GFX
    {

        namespace Async
        {

            OuterWorker:: OuterWorker(const Boundaries &b) :
            boundaries(b)
            {
            }


            OuterWorker:: ~OuterWorker() throw()
            {
            }


            void OuterWorker:: run(parallel &, lockable &sync)
            {
                Y_LOCK(sync);
                std::cerr << "OuterWorker" << std::endl;
            }

        }

    }

}
