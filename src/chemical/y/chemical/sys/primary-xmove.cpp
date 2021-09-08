#include "y/chemical/sys/primary.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {


        bool Primary:: update(Addressable &C,
                              const double x,
                              const Actor &a,
                              Addressable &xi) const throw()
        {
            tao::ld(xi,0);
            xi[(**this).indx] = x;
            tao::mul_add(C,NuT,xi);
            C[a.sp.indx] = 0;
            ensurePositive(C);
            return false;
        }


        bool Primary:: modify(Addressable &C, const double x, Addressable &xi) const throw()
        {
            xi[ (**this).indx ] = x;
            tao::mul_add(C,NuT,xi);
            ensurePositive(C);
            return true;
        }

        bool Primary:: xmoveByReac(Addressable &C, const double x, Addressable &xi) const throw()
        {
            double       xmax = 0;
            const Actor &amax = reac(xmax,C);
            if(x>xmax)
            {
                return update(C,xmax,amax,xi);
            }
            else
            {
                return modify(C,x,xi);
            }
        }

        bool Primary:: xmoveByProd(Addressable &C, const double x, Addressable &xi) const throw()
        {
            double       xmin = 0;
            const Actor &amin = prod(xmin,C);
            if(x<xmin)
            {
                return update(C,xmin,amin,xi);
            }
            else
            {
                return modify(C,x,xi);
            }
        }

        bool Primary:: xmoveByBoth(Addressable &C, const double x, Addressable &xi) const throw()
        {
            double       xmax = 0;
            const Actor &amax = reac(xmax,C);
            if(x>xmax)
            {
                return update(C,xmax,amax,xi);
            }
            else
            {
                double       xmin = 0;
                const Actor &amin = prod(xmin,C);
                if(x<xmin)
                {
                    return update(C,xmin,amin,xi);
                }
                else
                {
                    return modify(C,x,xi);
                }
            }
        }


        bool Primary:: xmove(Addressable &C, const double x, Addressable &xi) const throw()
        {
            tao::ld(xi,0);
            switch(kind)
            {
                case LimitedByNone: break;
                case LimitedByReac: return xmoveByReac(C,x,xi);
                case LimitedByProd: return xmoveByProd(C,x,xi);
                case LimitedByBoth: return xmoveByBoth(C,x,xi);
            }
            assert(LimitedByNone==kind);
            return modify(C,x,xi);
        }

    }

}
