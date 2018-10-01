//! \file

#ifndef Y_CHEM_KINETICS_INCLUDED
#define Y_CHEM_KINETICS_INCLUDED 1

#include "y/chem/equilibria.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"

namespace upsylon
{
    namespace Chemical
    {

        typedef math::ODE::Field<double>::Equation Kinetics;

        class Integrator
        {
        public:
            typedef math::ODE::DriverCK<double>::Type   Driver;
            typedef math::ODE::Field<double>::Callback  Callback;

            explicit Integrator();
            virtual ~Integrator() throw();

            Driver driver;

            void solve(array<double> &C,
                       const double  t0,
                       const double  t1,
                       const double  dt_max,
                       Kinetics     &kinetics,
                       Equilibria   &eqs);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Integrator);
            Kinetics damped;
            Callback normal;
            
            void computeDamped( array<double> &dCdt, double t, const array<double> &C );
            void computeNormal( array<double> &C, double t);
            Equilibria *pEqs;
            Kinetics   *pKin;
        };

    }
}

#endif

