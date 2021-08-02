#include "y/alchemy/equilibrium.hpp"
#include "y/exceptions.hpp"
#include "y/mkl/solve/zrid.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Alchemy
    {

        namespace
        {
            struct callEq
            {
                const Equilibrium        &eq;
                const accessible<double> &C;
                const double              K0;

                inline double operator()(double xi) const
                {
                    return eq.compute(K0,C,xi);
                }

            };
        }

        bool   Equilibrium:: solve(const double         K0,
                                   addressable<double> &C) const
        {


            static const unsigned HasForwardLimit = 0x01;
            static const unsigned HasReverseLimit = 0x02;
            static const unsigned HasMirrorLimits = HasForwardLimit | HasReverseLimit;

            const Extents extents  = findExtents(C);
            unsigned      exflags  = (extents.forward.index > 0) ? HasForwardLimit : 0;
            exflags               |= (extents.reverse.index > 0) ? HasReverseLimit : 0;

            const callEq    F  = { *this, C, K0 };
            triplet<double> xi = { 0,0,0 };
            triplet<double> eq = { 0,0,0 };


            switch(exflags)
            {

                case HasForwardLimit:
                    eq.c = F(xi.c=extents.forward.value);
                    xi.a = -1;
                    while( (eq.a = F(xi.a)) * eq.c > 0 )
                    {
                        xi.a *= 2.0;
                    }
                    break;

                case HasReverseLimit:
                    eq.a = F(xi.a = -extents.reverse.value);
                    xi.c = 1;
                    while( (eq.c = F(xi.c)) * eq.a > 0 )
                    {
                        xi.c *= 2.0;
                    }
                    break;

                case HasMirrorLimits:
                    eq.a = F(xi.a = -extents.reverse.value);
                    eq.c = F(xi.c =  extents.forward.value);
                    break;

                default:
                    throw exception("%s corrupted extent", *name);
            }


            zrid<double> zsolve;
            if(!zsolve.run(F,xi,eq))
            {
                return false;
            }

            const double Xi = xi.b;

            for(const Actor::Node *a=reac->head();a;a=a->next)
            {
                const Actor &r = **a;
                double      &c = C[r->indx];
                c = max_of<double>(0.0,c-r.nu*Xi);
            }

            for(const Actor::Node *a=prod->head();a;a=a->next)
            {
                const Actor &p = **a;
                double      &c = C[p->indx];
                c = max_of<double>(0.0,c+p.nu*Xi);
            }

            return true;
        }

    }
}

