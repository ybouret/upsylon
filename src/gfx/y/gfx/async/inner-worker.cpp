
#include "y/gfx/async/inner-worker.hpp"

namespace upsylon
{
    namespace GFX
    {

        namespace Async
        {

            InnerWorker:: InnerWorker(const Tile &t) :
            tile(t)
            {
            }


            InnerWorker:: ~InnerWorker() throw()
            {
            }


            void InnerWorker:: run(parallel &, lockable &sync)
            {
                Y_LOCK(sync);
                std::cerr << "InnerWorker@rank=" << tile.rank << std::endl;
            }
        }

    }

}
