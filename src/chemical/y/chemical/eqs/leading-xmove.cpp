#include "y/chemical/leading.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Chemical
    {
        
        void   Leading:: moveLimited(const double   x,
                                     Addressable   &C,
                                     Addressable   &xi,
                                     const Actor   &a) const throw()
        {
            xi[root.indx] = x;
            tao::mul_add(C,NuT,xi);
            C[a.sp.indx] = 0;
            ensurePositive(C);
        }
        
        void   Leading:: moveTotally(const double   x,
                                     Addressable   &C,
                                     Addressable   &xi) const throw()
        {
            xi[root.indx] = x;
            tao::mul_add(C,NuT,xi);
            ensurePositive(C);
        }

        bool   Leading:: moveLimitedByReac(const double   x,
                                           Addressable   &C,
                                           Addressable   &xi) const throw()
        {
            const Actor &amax = maxFromReac(C);
            if(x>=xmax)
            {
                moveLimited(xmax,C,xi,amax);
                return false;
            }
            else
            {
                moveTotally(x,C,xi);
                return true;
            }
        }
        
        
        
        bool   Leading:: moveLimitedByProd(const double   x,
                                           Addressable   &C,
                                           Addressable   &xi) const throw()
        {
            const Actor &amin = minFromProd(C);
            if(x<=xmin)
            {
                moveLimited(xmin,C,xi,amin);
                return false;
            }
            else
            {
                moveTotally(x,C,xi);
                return true;
            }
        }
        
        bool   Leading:: moveLimitedByBoth(const double   x,
                                           Addressable   &C,
                                           Addressable   &xi) const throw()
        {
            const Actor &amin = minFromProd(C);
            if(x<=xmin)
            {
                moveLimited(xmin,C,xi,amin);
                return false;
            }
            else
            {
                const Actor &amax = maxFromReac(C);
                if(x>=xmax)
                {
                    moveLimited(xmax,C,xi,amax);
                    return false;
                }
                else
                {
                    moveTotally(x,C,xi);
                    return true;
                }
            }
        }
        
        
        bool Leading:: tryMoveFull(const double   x,
                                   Addressable   &C,
                                   Addressable   &xi) const throw()
        {
            tao::ld(xi,0);
            
            switch(kind)
            {
                
                case LimitedByReac: return moveLimitedByReac(x,C,xi);
                case LimitedByProd: return moveLimitedByProd(x,C,xi);
                case LimitedByBoth: return moveLimitedByBoth(x,C,xi);
                case LimitedByNone:
                    break;
                    
            }
            assert(LimitedByNone==kind);
            moveTotally(x,C,xi);
            return true;
        }
        
    }
    
}


