#include "y/math/fit/ls.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            LeastSquares_:: ~LeastSquares_() throw()
            {
            }

            LeastSquares_:: LeastSquares_() throw()
            {
            }

            const char * LeastSquares_:: ConvergedText(const bool flag) throw()
            {
                return (flag?
                        "<  converged  >" :
                        "<not converged>");
            }

            const char * LeastSquares_:: YesOrNoText(const bool flag) throw()
            {
                return (flag? "yes" : "no ");
            }

            std::ostream & LeastSquares_:: OutputLine(std::ostream &os, size_t n)
            {
                while(n-->0) os << '_';
                return os;
            }
        }

    }

}

