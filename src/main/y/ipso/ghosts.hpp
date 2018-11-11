//! \file
#ifndef Y_IPSO_GHOSTS_INCLUDED
#define Y_IPSO_GHOSTS_INCLUDED 1

#include "y/ipso/ghost.hpp"

namespace upsylon
{
    namespace ipso
    {

        class ghosts
        {
        public:
            virtual ~ghosts() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ghosts);
        };

    }
}

#endif

