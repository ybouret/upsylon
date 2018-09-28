//! \file

#ifndef Y_CHEM_KINETICS_INCLUDED
#define Y_CHEM_KINETICS_INCLUDED 1

#include "y/chem/equilibria.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"

namespace upsylon
{
    namespace Chemical
    {

        class Integrator
        {
        public:
            typedef math::ODE::DriverCK<double>::Type Driver;

            explicit Integrator();
            virtual ~Integrator() throw();

            Driver driver;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Integrator);
        };

    }
}

#endif

