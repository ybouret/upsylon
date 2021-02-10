#include "y/gfx/async/worker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            static inline
            void noWork(Worker &worker, lockable &sync, void *)
            {
                Y_LOCK(sync);
                std::cerr << "worker@" << worker.label << std::endl;
            }

            Worker:: Worker(const size_t sz, const size_t rk, const Tile &t) :
            parallel(sz,rk),
            tile(t),
            kproc(noWork),
            kdata(0)
            {
            }

            Worker:: ~Worker() throw()
            {
            }

            void Worker:: run(lockable &sync)
            {
                kproc(*this,sync,kdata);
            }
            
        }
    }
}
