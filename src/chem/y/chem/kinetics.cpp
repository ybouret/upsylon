#include "y/chem/kinetics.hpp"


namespace upsylon
{
    namespace Chemical
    {
        Integrator:: ~Integrator() throw() {}

        Integrator:: Integrator() :
        driver(),
        damped(this, & Integrator::computeDamped),
        pEqs(0),
        pKin(0)
        {
        }

        void Integrator:: computeDamped(array<double> &dCdt, double t, const array<double> &C)
        {
            assert(pEqs);
            assert(pKin)
            (*pKin)(dCdt,t,C);
            pEqs->computeK(t);
            pEqs->damp(dCdt,C);
        }


        void Integrator:: solve(array<double> &C,
                                const double  t0,
                                const double  t1,
                                const double  dt_max,
                                Kinetics     &kinetics,
                                Equilibria   &eqs)
        {
            
        }
        
    }
}
