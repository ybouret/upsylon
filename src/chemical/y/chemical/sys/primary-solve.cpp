
#include "y/chemical/sys/primary.hpp"
#include "y/mkl/tao.hpp"
#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        static const size_t xwidth = 16;

        bool Primary:: solveLimitedByReac(Addressable &C,
                                          Addressable &xi) const throw()
        {
            double       xmax = 0;
            const Actor &amax = reac(xmax,C);
            Y_CHEMICAL_PRINTLN("      |_" << root << " max=" << std::setw(xwidth) << xmax << " <== " << amax.sp);

            if(xmax<0)
            {
                tao::ld(xi,0);
                xi[root.indx] = xmax;
                tao::mul_add(C,NuT,xi);
                C[amax.sp.indx] = 0;
            }

            return true;
        }


        bool Primary:: solveLimitedByProd(Addressable &C,
                                          Addressable &xi) const throw()
        {
            double       xmin = 0;
            const Actor &amin = prod(xmin,C);
            Y_CHEMICAL_PRINTLN("      |_" << root << " min=" << std::setw(xwidth) << xmin << " <== " << amin.sp);

            if(xmin>0)
            {
                tao::ld(xi,0);
                xi[root.indx] = xmin;
                tao::mul_add(C,NuT,xi);
                C[amin.sp.indx] = 0;
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
            Y_CHEMICAL_PRINTLN("      |_" << root << " min=" << std::setw(xwidth) << xmin << " <== " << amin.sp);
            Y_CHEMICAL_PRINTLN("      |_" << root << " max=" << std::setw(xwidth) << xmax << " <== " << amax.sp);

            if(xmin>xmax)
            {
                Y_CHEMICAL_PRINTLN("      |_" << root << " is invalid");
                return false;
            }
            else
            {
                if(xmin>0)
                {
                    tao::ld(xi,0);
                    xi[root.indx] = xmin;
                    tao::mul_add(C,NuT,xi);
                    C[amin.sp.indx] = 0;
                }

                if(xmax<0)
                {
                    tao::ld(xi,0);
                    xi[root.indx] = xmax;
                    tao::mul_add(C,NuT,xi);
                    C[amax.sp.indx] = 0;
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
