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

        void   Equilibrium:: solve(const double         K0,
                                   addressable<double> &C) const
        {


            static const unsigned HasForwardLimit = 0x01;
            static const unsigned HasReverseLimit = 0x02;
            static const unsigned HasMirrorLimits = HasForwardLimit | HasReverseLimit;

            const Extents extents = findExtents(C);
            unsigned      flag    = extents.forward.index > 0 ? HasForwardLimit : 0;
            flag                 |= extents.reverse.index > 0 ? HasReverseLimit : 0;
            std::cerr << "C: " << C << std::endl;
            std::cerr << "extents: " << extents << std::endl;

            const callEq F = { *this, C, K0 };

            triplet<double> xi = { -1, 0, 1 };
            triplet<double> eq = { 0, compute(K0,C), 0 };

            std::cerr << "xi=" << xi << std::endl;
            std::cerr << "eq=" << eq << " / " << F(0) << std::endl ;

            switch(flag)
            {
                case 0: std::cerr << name << " has no limit!" << std::endl;
                    break;

                case HasForwardLimit:
                    std::cerr << name << " has forward limit | " << extents.forward.value << std::endl;
                    break;

                case HasReverseLimit:
                    std::cerr << name << " has reverse limit | " << extents.reverse.value << std::endl;
                    
                    break;

                case HasMirrorLimits:
                    std::cerr << name << " has mirror limits | -" << extents.reverse.value << " -> " << extents.forward.value << std::endl;
                    break;

                default: throw exception("%s corrupted extent", *name);
            }


        }

    }
}

