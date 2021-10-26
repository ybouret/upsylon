
//! \file
#ifndef Y_CHEMICAL_REACTOR_INCLUDED
#define Y_CHEMICAL_REACTOR_INCLUDED 1

#include "y/chemical/equilibria.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        class Reactor
        {
        public:
            static const char CLID[];
            
            explicit Reactor(const Library    &,
                             const Equilibria &,
                             const unsigned);
            
            virtual ~Reactor() throw();
            
            const Library       &lib;        //!< (frozen) library
            const Equilibria    &eqs;        //!< (frozen) equilibria
            const size_t         N;          //!< number of equilibria
            const size_t         M;          //!< number of species
            const size_t         MW;         //!< working species [0..M]
            const size_t         Nc;         //!< complementary constraints
            const size_t         MS;         //!< spectators      [0..M]
            const iMatrix        Nu;         //!< [NxM] topology
            const iMatrix        NuT;        //!< [MxN] Nu'
            const iVector        Z;          //!< [M] vector of charges
            const bool           charged;    //!< |Z| != 0
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezable::Latch libLatch;
            const Freezable::Latch eqsLatch;
        };
    }
    
}

#endif
