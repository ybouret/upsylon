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
            const size_t   P;          //!< parameters = M-N
            const EqVector equilibria; //!< validated equilibria [N]
            iMatrix        Nu;         //!< topology             [NxM]
            iMatrix        tNu;        //!< transposed           [MxN]
            iMatrix        Nu2;        //!< Nu*tNu, Gram matrix  [NxN]
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
            Array         &Corg;       //!< original  C [M]
            Array         &Caux;       //!< auxiliary C [M]
            Array         &Ctry;       //!< trial     C [M]
            Array         &Cstp;       //!< step  for C [M]
            Array         &Cfwd;       //!< for forward [M]
            const Booleans active;     //!< active C    [M]


            //! balance C[1..M]
            bool balance(addressable<double> &) throw();

            //! forward C[1..M]
            bool forward(addressable<double> &) throw();

            void computeK(const double t);
            void computeQ(const accessible<double> &C) throw();
            void computePhi(const accessible<double> &C) throw();
            bool computeW() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            Array         &extra;  //!< for used    [M]
            Collector      keeper; //!< to keep all tidy


            double B_only(Array &C)         throw(); //!< uses Caux
            double B_drvs(Array &C)         throw(); //!< uses Caux for drvs
            double B_call(const double x)   throw(); //!< B_only(Ctry=Corg+x*Cstp)
            double sumCaux()                throw(); //!< sorted sum of Caux
            bool   rescale(const double B0) throw(); //!< rescale step
            struct B_proxy { Solver *self; double operator()(const double) throw(); };

        public:
            bool         balanceVerbose; //!< display status while balancing
        };

    }

}


#endif
