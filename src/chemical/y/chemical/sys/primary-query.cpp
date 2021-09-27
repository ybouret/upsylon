
#include "y/chemical/system.hpp"


namespace upsylon
{

    namespace Chemical
    {

        bool Primary:: queryForward(const Accessible &C) const throw()
        {
            switch(kind)
            {
                case LimitedByBoth:
                case LimitedByReac: {
                    double xmax = 0;
                    (void) reac(xmax,C);
                    return xmax>0;
                }

                case LimitedByNone:
                case LimitedByProd:
                    break;
            }
            return true;
        }


        bool Primary:: queryReverse(const Accessible &C) const throw()
        {
            switch(kind)
            {
                case LimitedByBoth:
                case LimitedByProd: {
                    double xmin = 0;
                    (void) prod(xmin,C);
                    return xmin<0;
                }

                case LimitedByNone:
                case LimitedByReac:
                    break;
            }
            return true;
        }

        



    }

}


