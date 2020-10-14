
#include "y/aqua/equilibrium.hpp"

namespace upsylon {

    namespace Aqua
    {

        Extents:: ~Extents() throw()
        {
        }

        Extents:: Extents(const Equilibrium        &eq,
                          const accessible<double> &C,
                          double                   *A) throw() :
        forward(C,eq.reactants,A),
        reverse(C,eq.products, A),
        blocked(forward.blocked&&reverse.blocked),
        reaches( (forward.limited ? ForwardLimited : ForwardEndless) | (reverse.limited ? ReverseLimited : ReverseEndless) )
        {
        }

        Extents:: Extents(const Extents &other) throw() :
        forward(other.forward),
        reverse(other.reverse),
        blocked(other.blocked),
        reaches(other.reaches)
        {

        }


        std::ostream & operator<<(std::ostream &os, const Extents &x)
        {
            os << "forward: " << x.forward << " | reverse: " << x.reverse;
            return os;
        }

        double Extents:: cut(const double value) const throw()
        {
            if(blocked)
            {
                return 0;
            }
            else
            {
                if(value>0)
                {
                    return forward.cut(value);
                }
                else
                {
                    if(value<0)
                    {
                        return -reverse.cut(-value);
                    }
                    else
                    {
                        return 0;
                    }
                }
            }

        }


    }

}
