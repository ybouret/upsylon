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

        typedef arrays<double> Arrays;
        typedef lightweight_array<double> Array;

        class Solver
        {
        public:
            explicit Solver();
            virtual ~Solver() throw();

            void init(Library &lib, Equilibria &eqs);
            void quit() throw();

            const size_t N;    //!< equilibria
            const size_t M;    //!< species
            Matrix       nu;   //!< topology [NxM]
            Arrays       arr;  //!< linear data
            Array       &Ctry; //!< trial C [M]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);

        };

    }

}


#endif
