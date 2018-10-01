#include "y/chem/kinetics.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Integrator:: ~Integrator() throw() {}

        Integrator:: Integrator() : Driver(),
        damped(this, & Integrator::computeDamped),
        normal(this, & Integrator::computeNormal),
        pEqs(0),
        pKin(0)
        {
        }

        void Integrator:: computeDamped(array<double> &dCdt, double t, const array<double> &C)
        {
            assert(pEqs);
            assert(pKin);
            (*pKin)(dCdt,t,C);
            pEqs->computeK(t);
            if(!pEqs->damp(dCdt,C))
            {
                throw exception("Integrator: cannot damp trial @t=%g!",t);
            }
        }


        void Integrator:: computeNormal(array<double> &C, double t)
        {
            assert(pEqs);
            pEqs->computeK(t);
            if(!pEqs->normalize(C))
            {
                throw exception("Integrator: cannot normalize trial @t=%g!",t);
            }
        }

        void Integrator:: solve(array<double> &C,
                                const double   t0,
                                const double   t1,
                                const double   dt_max,
                                Kinetics     &kinetics,
                                Equilibria   &eqs)
        {
            assert(t1>=t0);
            assert(dt_max>0);
            pKin = &kinetics;
            pEqs = &eqs;
            try
            {
                double h = min_of<double>(dt_max,t1-t0);
                (*this)(damped,C,t0,t1,h,&normal);
                pKin = 0;
                pEqs = 0;
            }
            catch(...)
            {
                pKin = 0;
                pEqs = 0;
                throw;
            }
        }
        
    }
}
