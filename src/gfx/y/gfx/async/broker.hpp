
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
            typedef arc_ptr<concurrent::server> SharedServer;


            //__________________________________________________________________
            //
            //
            //! gather a Tiling and a Server for ops
            //
            //__________________________________________________________________
            class Broker
            {
            public:
                //! cleanup
                virtual ~Broker() throw();
                //! setup
                explicit Broker(const SharedEngine &eng,
                                const SharedServer &srv) throw();

                SharedEngine engine; //!< shared engine
                SharedServer server; //!< shared server

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Broker);
            };

        }

    }
}

#endif