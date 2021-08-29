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
            virtual ~Reactor() throw();
            explicit Reactor(Library       &usrLib,
                             Equilibria    &usrEqs,
                             const unsigned flags);
            
            Library      &lib;
            Equilibria   &eqs;
            const size_t  M;
            const size_t  N;
            const size_t  NW;
            const size_t  NL;
            const size_t  NS;
            const iMatrix Nu;
            const iMatrix NuT;
            
        private:
            const Freezable::Latch lockLib;
            const Freezable::Latch lockEqs;
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };
    }
}

#endif

