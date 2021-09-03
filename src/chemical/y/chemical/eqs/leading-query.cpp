
#include "y/chemical/leading.hpp"

namespace upsylon
{
    namespace Chemical
    {

        bool Leading:: queryForward(const Accessible &C) const throw()
        {
            switch (kind)
            {
                case LimitedByReac:
                case LimitedByBoth:
                    (void) maxFromReac(C);
                    return xmax>0;
                    
                case LimitedByProd:
                case LimitedByNone:
                    break;
            }
            return true;
        }

        bool Leading:: queryReverse(const Accessible &C) const throw()
        {
            switch (kind)
            {
                case LimitedByProd:
                case LimitedByBoth:
                    (void) minFromProd(C);
                    return xmin<0;

                case LimitedByReac:
                case LimitedByNone:
                    break;
            }
            return true;
        }

    }

}

