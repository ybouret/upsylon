
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
            class Broker : public gateway<Team>
            {
            public:
                //! cleanup
                virtual ~Broker() throw();
                //! setup
                explicit Broker(const SharedEngine &eng,
                                const SharedServer &srv) throw();

                SharedEngine engine; //!< shared engine
                SharedServer server; //!< shared server

                //! dispatch the kernel and cycle engine
                void operator()( Worker::Kernel kproc, void *kdata );

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Broker);
                virtual const Team & bulk() const throw();
            };

        }

    }
}

#endif
