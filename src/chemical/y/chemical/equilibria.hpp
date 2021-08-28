
//! \file

#ifndef Y_CHEMICAL_EQUILIBRIA_INCLUDED
#define Y_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "y/chemical/equilibrium.hpp"

namespace upsylon
{
    namespace Chemical
    {

        class Equilibria : public Freezable, public gateway<const Equilibrium::Set>
        {
        public:
            static const char CLID[];
            
            explicit Equilibria();
            virtual ~Equilibria() throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual const_type& bulk() const throw();
            
            Equilibrium::Set edb;
        };
    }
}

#endif

