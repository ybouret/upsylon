#include "y/chemical/sys/primary.hpp"


namespace upsylon
{

    namespace Chemical
    {

        bool Primary:: search(double           &  x,
                              const Actor *    &  A,
                              const Accessible &  C) const throw()
        {
            x = 0;
            A = 0;

            switch(kind)
            {
                case LimitedByNone: // always successfull
                    break;

                case LimitedByReac: // always successfull, 0 or (xmax<0)
                {
                    double       xmax = 0;
                    const Actor &amax = reac(xmax,C);
                    if(xmax<0)
                    {
                        x =  xmax;
                        A = &amax;
                    }
                } return true;

                case LimitedByProd: // always successfull, 0 or (xmin>0)
                {
                    double       xmin = 0;
                    const Actor &amin = prod(xmin,C);
                    if(xmin>0)
                    {
                        x =  xmin;
                        A = &amin;
                    }
                } return true;

                case LimitedByBoth:
                {
                    double       xmin = 0;
                    const Actor &amin = prod(xmin,C);
                    double       xmax = 0;
                    const Actor &amax = reac(xmax,C);


                    if(xmin>xmax)
                    {
                        return false;
                    }
                    else
                    {
                        if(xmin>0)
                        {
                            x =  xmin;
                            A = &amin;
                        }

                        if(xmax<0)
                        {
                            x =  xmax;
                            A = &amax;
                        }

                        return true;
                    }
                }

            }

            assert(LimitedByNone==kind);
            return true;
        }
    }

}

