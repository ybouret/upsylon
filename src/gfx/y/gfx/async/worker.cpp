#include "y/gfx/async/worker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            void Worker:: None(Worker &worker, lockable &sync, void *)
            {
                Y_LOCK(sync);
                std::cerr << "worker@" << worker.label << " : " << worker.tile << std::endl;
            }

            Worker:: Worker(const size_t sz, const size_t rk, const Tile &t) :
            parallel(sz,rk),
            tile(t),
            kproc(None),
            kdata(0)
            {
            }

            Worker:: ~Worker() throw()
            {
            }

            void Worker:: run(lockable &sync)
            {
                assert(kproc);
                kproc(*this,sync,kdata);
            }

            Point Worker:: begin() const throw()
            {
                return tile[1].begin;
            }

            void Worker:: load(Kernel k,void *d) throw()
            {
                assert(k);
                kproc = k;
                kdata = d;
            }

            
        }
    }
}
