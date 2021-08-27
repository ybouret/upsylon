
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/type/utils.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {
        static inline double u2d(const unit_t u)
        {
            return double(u);
        }

        size_t Reactor:: seekingSpecies(Addressable &Cbad, const Accessible &C) const throw()
        {
            assert(NS==Cbad.size());
            size_t nbad = 0;
            for(size_t j=NS;j>0;--j)
            {
                const double Cj = C[ seeking[j]->indx ];
                if(Cj<=0)
                {
                    Cbad[j] = -Cj;
                    ++nbad;
                }
                else
                {
                    Cbad[j] = 0;
                }
            }
            return nbad;
        }


        bool Reactor:: balanceSeeking(Addressable &C)
        {
            bool balanced = true;

            if(Verbosity)
            {
                std::cerr << "<Balance Seeking>" << std::endl;
                showConditions(std::cerr,C);
            }

            if(NS>0)
            {
                Vector         Cb(NS,0);
                Addressable   &Xi = aliasing::_(xi);
                Y_ALCHEM_PRINTLN("Vs="<<Vs);

                matrix<double> Vm(NS,N);
                matrix<double> tV(N,NS);
                matrix<double> V2(NS,NS);
                Vm.assign(Vs,u2d);
                tV.assign_transpose(Vm);
                tao::gram(V2,Vm);
                Y_ALCHEM_PRINTLN("V2="<<V2);
                if(!LU::build(V2))
                {
                    Y_ALCHEM_PRINTLN("  singular seeking matrix");
                    balanced = false;
                    goto DONE;
                }
                const size_t nb = seekingSpecies(Cb,C);
                Y_ALCHEM_PRINTLN("Cb="<<Cb);
                if(nb>0)
                {
                    LU::solve(V2,Cb);
                    tao::mul(Xi,tV,Cb);
                    Y_ALCHEM_PRINTLN("Xi="<<Xi);

                }


            }

        DONE:
            if(Verbosity)
            {
                std::cerr << "  ==> [" <<textual::boolean(balanced) << "] <==" << std::endl;
                std::cerr << "<Balanced Seeking/>" << std::endl;
            }

            return balanced;
        }
    }

}

