
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

            void Broker:: operator()( Worker::Routine kproc, void *kdata )
            {
                assert(kproc);
                engine->cycle(*server,kproc,kdata);
            }
            
            const Team & Broker:: bulk() const throw()
            {
                return *engine;
            }

        }
    }
}

