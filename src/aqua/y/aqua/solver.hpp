//! \file

#ifndef Y_AQUA_SOLVER_INCLUDED
#define Y_AQUA_SOLVER_INCLUDED 1

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/sequence/arrays.hpp"
#include "y/core/temporary-acquire.hpp"

namespace upsylon
{

    namespace Aqua
    {

        typedef arrays<double>              Arrays;     //!< alias
        typedef array<double>               Array;      //!< alias
        typedef lightweight_array<bool>     Booleans;   //!< alias

        //! solver
        class Solver
        {
        public:
            typedef core::temporary_acquire<16> Collector;

            explicit Solver();
            virtual ~Solver() throw();

            void init(Library &lib, Equilibria &eqs);
            void quit() throw();

            const size_t   N;      //!< equilibria
            const size_t   M;      //!< species
            const size_t   A;      //!< active species
            const size_t   P;      //!< parameters = M-N
            const int      dNu2;   //!< det(Nu2)
            iMatrix        Nu;     //!< topology      [NxM]
            iMatrix        tNu;    //!< transposed    [MxN]
            iMatrix        Nu2;    //!< Nu*tNu        [NxN]
            Matrix         Phi;    //!< Jacobian      [NxM]
            Matrix         W;      //!<               [NxN]
            Arrays         aN;     //!< linear data   [N]...
            Array         &K;      //!< constants     [N]
            Array         &Q;      //!< indicators    [N]
            Array         &xi;     //!< extent        [N]
            Array         &nu2;    //!< sum(Nu_i^2)   [N]
            Arrays         aM;     //!< linear data   [M]...
            Array         &Corg;   //!< original  C [M]
            Array         &Caux;   //!< auxiliary C [M]
            Array         &Ctry;   //!< trial     C [M]
            Array         &Cstp;   //!< step  for C [M]
            Array         &Cfwd;   //!< for forward [M]
            Array         &tmp_;   //!< for used    [M]
            const Booleans active; //!< active C    [M]
            Collector      clr;


            bool balance( addressable<double> &C ) throw();
            bool forward(const Equilibria &eqs, addressable<double> &C ) throw();

            void computeK(const Equilibria &eqs, const double t);
            void computeQ(const Equilibria &eqs, const accessible<double> &C) throw();
            void computePhi(const Equilibria &eqs, const accessible<double> &C) throw();
            bool computeW() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            double B_only(Array &C)         throw(); //!< uses Caux
            double B_drvs(Array &C)         throw(); //!< uses Caux for drvs
            double B_call(const double x)   throw(); //!< B_only(Ctry=Corg+x*Cstp)
            double sumCaux()                throw(); //!< sorted sum of Caux
            bool   rescale(const double B0) throw(); //!< rescale step
            struct B_proxy { Solver *self; double operator()(const double) throw(); };

        public:
            bool         balanceVerbose;
        };

    }

}


#endif
