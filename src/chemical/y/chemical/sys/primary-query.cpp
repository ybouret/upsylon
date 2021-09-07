
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

        

    }

}


