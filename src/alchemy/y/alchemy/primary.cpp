

#include "y/alchemy/primary.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        Sentry::State Sentry:: solveHasOnlyGEQ(Addressable   &C,
                                      const iMatrix &NuT,
                                      Vector        &xi) const throw()
        {
            assert(HasOnlyGEQ==type);
            const Primary &p = getMin(C);
            const double   c = C[p.sp];
            if(c<0)
            {
                tao::ld(xi,0);
                xi[p.eq] = -c/p.nu;
                std::cerr << "xi=" << xi << std::endl;
                tao::mul_add(C,NuT,xi);
                std::cerr << "C=" << C << std::endl;
                C[p.sp] = 0;
                return WasMoved;
            }
            else
            {
                return WasValid;
            }
        }

        Sentry::State Sentry:: solveHasOnlyLEQ(Addressable &C, const iMatrix &NuT, Vector &xi ) const throw()
        {
            assert(HasOnlyLEQ==type);
            const Primary &p = getMax(C);
            const double   c = C[p.sp];
            if(c<0)
            {
                tao::ld(xi,0);
                xi[p.eq] = c/p.nu;
                std::cerr << "xi=" << xi << std::endl;
                tao::mul_add(C,NuT,xi);
                std::cerr << "C=" << C << std::endl;
                C[p.sp] = 0;
                return WasMoved;
            }
            else
            {
                return WasValid;
            }
        }


        Sentry::State Sentry:: solveIsBothWays(Addressable &C, const iMatrix &NuT, Vector &xi ) const throw()
        {
            assert(IsBothWays==type);

            const Primary &pmax = getMax(C);
            const double   xmax = C[pmax.sp]/pmax.nu;

            const Primary &pmin = getMin(C);
            const double   xmin = -C[pmin.sp]/pmin.nu;

            std::cerr << "xmin=" << xmin << ", xmax=" << xmax << std::endl;

            if(xmax<=xmin)
            {
                return IsJammed;
            }
            else
            {
                if(xmax<0)
                {
                    tao::ld(xi,0);
                    xi[pmax.eq] = xmax;
                    std::cerr << "xi=" << xi << std::endl;
                    tao::mul_add(C,NuT,xi);
                    std::cerr << "C=" << C << std::endl;
                    C[pmax.sp] = 0;
                    return WasMoved;
                }
                else
                {
                    if(xmin>0)
                    {
                        xi[pmin.eq] = xmin;
                        std::cerr << "xi=" << xi << std::endl;
                        tao::mul_add(C,NuT,xi);
                        std::cerr << "C=" << C << std::endl;
                        C[pmin.sp] = 0;
                        return WasMoved;
                    }
                    else
                    {
                        return WasValid;
                    }
                }
            }
        }


    }

}

