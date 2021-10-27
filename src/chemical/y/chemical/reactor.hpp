
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
            const Flags          active;     //!< [M] active species
            Vector               K;          //!< [N]
            Vector               Gamma;      //!< [N] equilibria
            Matrix               J;          //!< [NxM] jacobian of Gamma
            Matrix               JNuT;       //!< [NxN]
            Vector               xi;         //!< [N] extent
            Vector               startC;     //!< [M] startC
            Vector               deltaC;     //!< [M] deltaC
            Vector               trialC;     //!< [M] trialC
            const bool           charged;    //!< |Z| != 0


            void loadK(const double t);                         //!< load constants at time t
            void computeGamma(const Accessible &C) throw();     //!< compute Gamma for pre-computed K
            void computeGammaAndJ(const Accessible &C) throw(); //!< compute Gamma and J for pre-computed K
            bool computeImpulse(const Accessible &C) throw();   //!< compute LU decomp of (J*NuT) for pre-computed K
            bool computeFullStep() throw(); //! @startC

            bool solve(Addressable &C); //!< solve for precomputed K


            bool damp(Addressable &rate, const Addressable &C);




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezable::Latch libLatch;
            const Freezable::Latch eqsLatch;
        };
    }
    
}

#endif
