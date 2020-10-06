//! \file

#ifndef Y_AQUA_SOLVER_INCLUDED
#define Y_AQUA_SOLVER_INCLUDED 1

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/sequence/arrays.hpp"
#include "y/core/temporary-acquire.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{

    namespace Aqua
    {

        typedef arrays<double>                     Arrays;     //!< alias
        typedef array<double>                      Array;      //!< alias
        typedef lightweight_array<bool>            Booleans;   //!< alias
        typedef vector<const Equilibrium::Pointer> EqVector;   //!< alias
        class                                      Boot;       //!< forward
        
        //! solver
        class Solver
        {
        public:
            //__________________________________________________________________
            //
            // types and defintiions
            //__________________________________________________________________
            typedef core::temporary_acquire<16> Collector; //!< alias


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Solver();          //!< setup
            virtual ~Solver() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void init(Library &lib, const Equilibria &eqs); //!< initialize
            void quit() throw();                            //!< release all

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            const size_t   N;          //!< equilibria
            const size_t   M;          //!< species
            const size_t   A;          //!< active species
            const size_t   Nc;         //!< parameters = M-N
            const EqVector equilibria; //!< validated equilibria [N]
            const iMatrix  Nu;         //!< topology             [NxM]
            const iMatrix  tNu;        //!< transposed           [MxN]
            const iMatrix  Nu2;        //!< Nu*tNu, Gram matrix  [NxN]
            const int      det;        //!< det(Nu2), check independant equilibria
            Matrix         Phi;        //!< Jacobian      [NxM]
            Matrix         W;          //!< Phi*tNu       [NxN]
        private:
            Arrays         aN;         //!< linear data   [N]...
        public:
            Array         &K;          //!< constants     [N]
            Array         &Q;          //!< indicators    [N]
            Array         &xi;         //!< extent        [N]
            Array         &nu2;        //!< sum(Nu_i^2)   [N]
        private:
            Arrays         aM;         //!< linear data   [M]...
        public:
            Array         &Corg;       //!< original  C   [M]
            Array         &Caux;       //!< auxiliary C   [M]
            Array         &Ctry;       //!< trial     C   [M]
            Array         &Cstp;       //!< step  for C   [M]
            Array         &Cini;       //!< for forward   [M]
            Array         &Cend;       //!< forward trial [M]
            Array         &Cswp;       //!< to sweep
            const Booleans active;     //!< active C      [M]


            //! balance C[1..M]
            bool balance(addressable<double> &) throw();

            //! forward C[1..M]
            bool forward(addressable<double> &) throw();

            //! compute K[1..N] at time t
            void computeK(const double t);

            //! compute Q[1..N] from K and C
            void computeQ(const accessible<double> &C) throw();

            //! compute Q[1..N] and Phi[NxM] from K and C
            void computeS(const accessible<double> &C) throw();

            //! compute Phi*tNu, return LU result
            bool computeW() throw();

            //! try to sweep equilibrium i in [1..N]
            bool sweep(addressable<double> &C, const size_t i) throw();

            //! try to move equilibria by sweeping
            /**
             try all permutations ?
             */
            bool swept(addressable<double> &C) throw();

            //! try to boot
            void boot(addressable<double> &C, Boot &boot);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            Array         &extra;  //!< for used    [M]
            Collector      keeper; //!< to keep all tidy


            double B_only(Array &C)         throw(); //!< uses Caux
            double B_drvs(Array &C)         throw(); //!< uses Caux for drvs
            double B_call(const double x)   throw(); //!< B_only(Ctry=Corg+x*Cstp)
            double sumCaux()                throw(); //!< sorted sum of Caux
            bool   rescale(const double B0) throw(); //!< rescale balancing step

            struct B_proxy { Solver *self; double operator()(const double) throw(); };

            double Q_only(const Array &C) throw(); //!< objective function of Q
            double Q_call(const double x) throw(); //!< Q_only(Ctry=Cini+x*Cstp)
            struct Q_proxy { Solver *self; double operator()(const double) throw(); };


        public:
            bool         balanceVerbose;     //!< display status while balancing
            size_t       lastBalanceCycles;  //!< last cycles in balance
            bool         forwardVerbose;     //!< display status while forwarding
            size_t       lastForwardCycles;  //!< last cycles in forward, >=1
        };

    }

}


#endif
