#include "y/chemical/sys/primary.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {


        void Primary:: update(Addressable &C,
                              const double x,
                              const Actor &a,
                              Addressable &xi) const throw()
        {
            tao::ld(xi,0);
            xi[(**this).indx] = x;
            tao::mul_add(C,NuT,xi);
            C[a.sp.indx] = 0;
            ensurePositive(C);
        }


        void Primary:: modify(Addressable &C, const double x, Addressable &xi) const throw()
        {
            xi[ (**this).indx ] = x;
            tao::mul_add(C,NuT,xi);
            ensurePositive(C);
        }

        void Primary:: move(Addressable &C, const double x, Addressable &xi) const throw()
        {
            tao::ld(xi,0);
            switch(kind)
            {
                case LimitedByNone:
                    modify(C,x,xi);
                    break;

                case LimitedByReac: {
                    double       xmax = 0;
                    const Actor &amax = reac(xmax,C);
                    if(x>xmax)
                    {
                        update(C,xmax,amax,xi);
                    }
                    else
                    {
                        modify(C,x,xi);
                    }
                } break;

                case LimitedByProd: {
                    double       xmin = 0;
                    const Actor &amin = prod(xmin,C);
                    if(x<xmin)
                    {
                        update(C,xmin,amin,xi);
                    }
                    else
                    {
                        modify(C,x,xi);
                    }
                } break;

                case LimitedByBoth: {
                    double       xmax = 0;
                    const Actor &amax = reac(xmax,C);
                    if(x>xmax)
                    {
                        update(C,xmax,amax,xi);
                    }
                    else
                    {
                        double       xmin = 0;
                        const Actor &amin = prod(xmin,C);
                        if(x<xmin)
                        {
                            update(C,xmin,amin,xi);
                        }
                        else
                        {
                            modify(C,x,xi);
                        }
                    }
                } break;


            }
            
        }

    }

}
