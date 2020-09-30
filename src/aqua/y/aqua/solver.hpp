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

        typedef arrays<double>              Arrays;
        typedef lightweight_array<double>   Array;
        typedef lightweight_array<bool>     Booleans;
        typedef matrix<bool>                bMatrix;

        class Solver
        {
        public:
            typedef core::temporary_acquire<16> Collector;

            explicit Solver();
            virtual ~Solver() throw();

            void init(Library &lib, Equilibria &eqs);
            void quit() throw();

            const size_t N;      //!< equilibria
            const size_t M;      //!< species
            const size_t A;      //!< active species
            const size_t P;      //!< parameters = M-N
            const int    dNu2;   //!< det(Nu2)
            iMatrix      Nu;     //!< topology      [NxM]
            iMatrix      tNu;    //!< transposed    [MxN]
            iMatrix      Nu2;    //!< Nu*tNu        [NxN]
            iMatrix      aNu2;   //!< adjoint(Nu2)  [NxN]
            iMatrix      Proj;   //!< tNu*aNu2*Nu   [MxM]
            iMatrix      NGS;    //!< orthogonal Nu [NxM]
            iMatrix      Rho;    //!< orthogonal    [PxM]
            Matrix       W;      //!<               [NxN]
            Arrays       aN;     //!< linear data
            Array       &B;      //!< balance indicators [N]
            Array       &xi;     //!< extent             [N]
            Array       &nu2;    //!< scaling            [N]
            Arrays       aM;     //!< linear data
            Array       &Corg;   //!< original  C [M]
            Array       &Caux;   //!< auxiliary C [M]
            Array       &Ctry;   //!< trial     C [M]
            Array       &Cstp;   //!< step  for C [M]
            Array       &tmp_;   //!< for used    [M]
            Booleans     used;   //!< active C    [M]
            Collector    clr;


            bool balance( addressable<double> &C ) throw();
            bool balance2( addressable<double> &C ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            double B_only( const Array &C ) throw(); //!< uses Caux
            double B_drvs( const Array &C ) throw(); //!< uses Caux for drvs
            double B_call( const double x ) throw(); //!< B_only(Ctry=Corg+x*Cstp)
            double sumCaux()                throw(); //!< sorted sum of Caux
            bool   rescale()                throw(); //!< rescale step
            struct B_proxy { Solver *self; double operator()(const double) throw(); };

        public:
            bool         balanceVerbose;
        };

    }

}


#endif
