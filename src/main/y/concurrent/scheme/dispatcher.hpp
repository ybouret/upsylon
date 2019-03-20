//! \file

#ifndef Y_CONCURRENT_DISPATCHER_INCLUDED
#define Y_CONCURRENT_DISPATCHER_INCLUDED 1

#include "y/concurrent/scheme/server.hpp"
#include "y/concurrent/threads.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class dispatcher : public server
        {
        public:
            explicit dispatcher( const bool v = false );
            virtual ~dispatcher() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(dispatcher);

            threads workers;

        public:
            bool &verbose;
        };

    }

}

#endif
