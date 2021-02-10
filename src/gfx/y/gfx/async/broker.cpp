
#include "y/gfx/async/broker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {

            Broker:: ~Broker() throw()
            {
            }

            Broker:: Broker(const SharedEngine &eng,
                            const SharedServer &srv) throw()
            : engine(eng), server(srv)
            {

            }

            void Broker:: operator()( Worker::Kernel kproc, void *kdata )
            {
                assert(kproc);
                engine->cycle(*server,kproc,kdata);
            }

        }
    }
}

