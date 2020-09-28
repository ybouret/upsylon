//! \file

#ifndef Y_AQUA_SOLVER_INCLUDED
#define Y_AQUA_SOLVER_INCLUDED 1

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"

namespace upsylon
{

    namespace Aqua
    {


        class Solver
        {
        public:
            explicit Solver();
            virtual ~Solver() throw();

            void init(Library &lib, Equilibria &eqs);
            void quit() throw();

            const size_t N;  //!< equilibria
            const size_t M;  //!< species
            Matrix       nu; //!< topology [NxM]


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);

        };

    }

}


#endif
