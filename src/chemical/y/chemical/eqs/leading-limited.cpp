#include "y/chemical/leading.hpp"
#include "y/mkl/tao.hpp"
#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {
        static const unsigned xwidth = 16;

        Leading::Status Leading:: limitedByReac(Addressable   &C,
                                                const iMatrix &NuT,
                                                Addressable   &xi) const throw()
        {
            const Actor &amax = maxFromReac(C);
            Y_CHEMICAL_PRINTLN( "    |_max=" << std::setw(xwidth) << xmax << " from " << amax.sp << " = " << C[amax.sp.indx]);
            if(xmax<0)
            {
                tao::ld(xi,0);
                xi[root->indx] = xmax;
                tao::mul_add(C,NuT,xi);
                C[amax.sp.indx] = 0;
                return Modified;
            }
            else
            {
                return Accepted;
            }
        }

        Leading::Status Leading:: limitedByProd(Addressable   &C,
                                                const iMatrix &NuT,
                                                Addressable   &xi) const throw()
        {
            const Actor &amin = minFromProd(C);
            Y_CHEMICAL_PRINTLN("    |_min=" << std::setw(xwidth) << xmin << " from " << amin.sp << " = " << C[amin.sp.indx]);
            if(xmin>0)
            {
                tao::ld(xi,0);
                xi[root->indx] = xmin;
                tao::mul_add(C,NuT,xi);
                C[amin.sp.indx] = 0;
                return Modified;
            }
            else
            {
                return Accepted;
            }
        }

        Leading::Status Leading:: limitedByBoth(Addressable   &C,
                                                const iMatrix &NuT,
                                                Addressable   &xi) const throw()
        {
            const Actor &amax = maxFromReac(C);
            const Actor &amin = minFromProd(C);
            Y_CHEMICAL_PRINTLN("    |_min=" << std::setw(xwidth) << xmin << " from " << amin.sp << " = " << C[amin.sp.indx]);
            Y_CHEMICAL_PRINTLN("    |_max=" << std::setw(xwidth) << xmax << " from " << amax.sp << " = " << C[amax.sp.indx]);

            if(xmin>xmax)
            {
                return Rejected;
            }
            else
            {
                if(xmin>0)
                {
                    tao::ld(xi,0);
                    xi[root->indx] = xmin;
                    tao::mul_add(C,NuT,xi);
                    C[amin.sp.indx] = 0;
                    return Modified;
                }

                if(xmax<0)
                {
                    tao::ld(xi,0);
                    xi[root->indx] = xmax;
                    tao::mul_add(C,NuT,xi);
                    C[amax.sp.indx] = 0;
                    return Modified;
                }

                return Accepted;
            }


        }



        Leading::Status Leading:: solve(Addressable   &C,
                                        const iMatrix &NuT,
                                        Addressable   &xi) const throw()
        {
            Y_CHEMICAL_PRINTLN("    " << root);
            Leading::Status status = Rejected;
            switch(kind)
            {
                case LimitedByNone: status = Accepted; break;
                case LimitedByReac: status = limitedByReac(C,NuT,xi); break;
                case LimitedByProd: status = limitedByProd(C,NuT,xi); break;
                case LimitedByBoth: status = limitedByBoth(C,NuT,xi); break;
            }
            Y_CHEMICAL_PRINTLN("    |_<" << StatusText(status) << ">" << std::endl);
            return status;
        }

    }
}
