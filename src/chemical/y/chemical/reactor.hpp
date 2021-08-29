//! \file

#ifndef Y_CHEMICAL_REACTOR_INCLUDED
#define Y_CHEMICAL_REACTOR_INCLUDED 1

#include "y/chemical/equilibria.hpp"


namespace upsylon
{
    
    namespace Chemical
    {
        
        class Reactor : public Object
        {
        public:
            virtual ~ Reactor() throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };
    }
}

#endif

