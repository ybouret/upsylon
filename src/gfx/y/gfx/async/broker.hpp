
//! \file

#ifndef Y_GFX_ASYNC_BROKER_INCLUDED
#define Y_GFX_ASYNC_BROKER_INCLUDED 1


#include "y/gfx/async/engine.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {

            //__________________________________________________________________
            //
            //! alias for Sequential/Parallel server
            //__________________________________________________________________
            typedef arc_ptr<concurrent::server> Server;


            //__________________________________________________________________
            //
            //
            //! gather a Tiling and a Server for ops
            //
            //__________________________________________________________________
            class Broker
            {
            public:
                virtual ~Broker() throw();
                explicit Broker(const Tiling &tiling,
                                const Server &server) throw();

                Tiling tiling;
                Server server;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Broker);
            };

        }

    }
}

#endif
