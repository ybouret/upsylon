
#include "y/chemical/sys/primary.hpp"
#include "y/mkl/tao.hpp"
#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        static const size_t xwidth = 16;

        static inline void makePositive(double &c) throw()
        {
            c = max_of(c,0.0);
        }

        void Primary:: ensurePositive(Addressable &C) const throw()
        {
            for(size_t i=reac.size();i>0;--i)
            {
                makePositive( C[ reac[i].sp.indx ] );
            }

            for(size_t i=prod.size();i>0;--i)
            {
                makePositive( C[ prod[i].sp.indx ] );
            }
        }

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

        
        bool Primary:: solveLimitedByReac(Addressable &C,
                                          Addressable &xi) const throw()
        {
            double       xmax = 0;
            const Actor &amax = reac(xmax,C);
            Y_CHEMICAL_PRINTLN("      |_" << **this << " max=" << std::setw(xwidth) << xmax << " <== " << amax.sp);

            if(xmax<0)
            {
                update(C,xmax,amax,xi);
            }

            return true;
        }


        bool Primary:: solveLimitedByProd(Addressable &C,
                                          Addressable &xi) const throw()
        {
            double       xmin = 0;
            const Actor &amin = prod(xmin,C);
            Y_CHEMICAL_PRINTLN("      |_" << **this << " min=" << std::setw(xwidth) << xmin << " <== " << amin.sp);

            if(xmin>0)
            {
                update(C,xmin,amin,xi);
            }

            return true;
        }

        bool Primary:: solveLimitedByBoth(Addressable &C,
                                          Addressable &xi) const throw()
        {
            double       xmin = 0;
            const Actor &amin = prod(xmin,C);
            double       xmax = 0;
            const Actor &amax = reac(xmax,C);
            Y_CHEMICAL_PRINTLN("      |_" << **this << " min=" << std::setw(xwidth) << xmin << " <== " << amin.sp);
            Y_CHEMICAL_PRINTLN("      |_" << **this << " max=" << std::setw(xwidth) << xmax << " <== " << amax.sp);

            if(xmin>xmax)
            {
                Y_CHEMICAL_PRINTLN("      |_" << **this << " is invalid");
                return false;
            }
            else
            {
                if(xmin>0)
                {
                    update(C,xmin,amin,xi);
                }

                if(xmax<0)
                {
                    update(C,xmax,amax,xi);
                }

                return true;
            }

        }



        bool Primary:: solve(Addressable &C,
                             Addressable &xi) const throw()
        {
            if(Verbosity)
            {
                prolog(std::cerr,6);
            }
            bool result = true;
            switch(kind)
            {
                case LimitedByNone:
                    break;

                case LimitedByReac: result = solveLimitedByReac(C,xi); break;
                case LimitedByProd: result = solveLimitedByProd(C,xi); break;
                case LimitedByBoth: result = solveLimitedByBoth(C,xi); break;
            }
            
            return result;
        }

    }

}
