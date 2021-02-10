#include "y/gfx/async/worker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {

            Worker:: Worker(const size_t sz, const Tile &t) : parallel(sz,t.rank), tile(t)
            {
            }

            Worker:: ~Worker() throw()
            {
            }

            void Worker:: run(lockable &sync)
            {
                Y_LOCK(sync);
                std::cerr << "worker@tile#" << label << std::endl;
            }
            
        }
    }
}
