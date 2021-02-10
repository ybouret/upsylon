#include "y/gfx/async/worker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {

            Worker:: Worker(const Tile &t) : Cache(), tile(t)
            {
            }

            Worker:: ~Worker() throw()
            {
            }

            void Worker:: run(parallel &, lockable &sync)
            {
                Y_LOCK(sync);
                std::cerr << "worker@tile#" << tile.rank << std::endl;
            }
            
        }
    }
}
