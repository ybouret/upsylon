
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
                size_t Nx = N;     //! number of xi
                Flags  Fx(N,true);
                matrix<double> V2(NS,NS);
                Vector         Cs(NS,0);
                size_t         bad=0;
                for(size_t j=NS;j>0;--j)
                {
                    const double Cj =C[ seeking[j]->indx ];
                    if(Cj<0)
                    {
                        Cs[j] = -Cj;
                        ++bad;
                    }
                }

                std::cerr << "bad = " << bad << std::endl;
                std::cerr << "Cs  = " << Cs  << std::endl;

                if(bad)
                {
                    matrix<double> Vm(NS,Nx);
                    for(size_t i=1;i<=NS;++i)
                    {
                        size_t j=0;
                        for(size_t k=1;k<=N;++k)
                        {
                            if(Fx[k])
                            {
                                Vm[i][++j] = Vs[i][k];
                            }
                        }
                    }
                    std::cerr << "Vm  = " << Vm << std::endl;
                    tao::gram(V2,Vm);
                    std::cerr << "V2  = " << V2 << std::endl;
                    if(!LU::build(V2))
                    {
                        balanced = false; goto DONE;
                    }
                    LU::solve(V2,Cs);
                    Vector Xi(Nx,0);
                    tao::mul_trn(Xi,Vm,Cs);
                    std::cerr << "Xi  = " << Xi << std::endl;



                }
                else
                {
                    goto DONE;
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

