//! \file

#ifndef Y_AQUA_SOLVER_INCLUDED
#define Y_AQUA_SOLVER_INCLUDED 1

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/sequence/arrays.hpp"
namespace upsylon
{

    namespace Aqua
    {

        typedef arrays<double>            Arrays;
        typedef lightweight_array<double> Array;
        typedef lightweight_array<bool>   Booleans;

        class Solver
        {
        public:
            explicit Solver();
            virtual ~Solver() throw();

            void init(Library &lib, Equilibria &eqs);
            void quit() throw();

            const size_t N;    //!< equilibria
            const size_t M;    //!< species

            iMatrix      Nu;   //!< topology   [NxM]
            iMatrix      tNu;  //!< transposed [MxN]
            iMatrix      Nu2;  //!< tNu*Nu     [MxM]
            Matrix       W;    //!< [NxN]
            Arrays       arrN; //!< linear data
            Array       &xi;   //!< xi [N]

            Arrays       arrM; //!< linear data
            Array       &Corg; //!< original  C [M]
            Array       &Caux; //!< auxiliary C [M]
            Array       &Ctry; //!< trial     C [M]
            Array       &Cstp; //!< step  for C [M]
            Array       &Cusr; //!< for used    [M]
            Booleans     used; //!< active C    [M]

            bool balance( addressable<double> &C ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            double Bfunc(const Array &C) throw(); //!< compute B(C)
            double Bdrvs(const Array &C) throw(); //!< compute B(C) using Cayx, and unscaled Cstp using Ctry
            double Norm2(const Array &C) throw(); //!< |C|^2 using Caux
            double Bcall(const double x) throw(); //!< B(Ctry=Corg+x*Cstp)

            struct BalanceProxy { Solver *self; double operator()(const double) throw(); };
        };

    }

}


#endif
