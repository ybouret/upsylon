#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"
#include "y/alea.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        
        bool Reactor:: balance1(Addressable &C) throw()
        {
            assert(N>0);
            assert(NA>0);
            bool balanced = true;
            {
                Addressable &Xi = aliasing::_(xi); assert(N==xi.size());
                for(const Equilibrium::Node *node = eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    eqs.print(std::cerr << "guard ",eq) << std::endl;
                    const Guard::State st = guards[eq.indx]->solve(C,NuT,Xi,aliasing::_(ok));
                    std::cerr << "  " << Guard::StateText(st) << std::endl;
                    if(Guard::IsJammed==st)
                    {
                        balanced = false;
                    }
                }
            }
            
            lib.display(std::cerr << "Cbal=", C) << std::endl;
            return balanced;
        }


        bool Reactor:: balance(Addressable &C) throw()
        {
            lib.display(std::cerr << "C=",C) << std::endl;
            tao::ld( aliasing::_(ok),true);
            if(N>0)
            {
                assert(NA>0);
                //______________________________________________________________
                //
                // starting point: need to be reduced
                // _____________________________________________________________
                showConditions(std::cerr,C);

                if( !balance1(C) )
                {
                    lib.display(std::cerr << "ok=", ok)  << std::endl;
                    return false;
                }
                std::cerr << "Balanced Leading!" << std::endl;
           
                //______________________________________________________________
                //
                // reduced starting point
                // _____________________________________________________________
                showConditions(std::cerr,C);
                std::cerr << "Seeking:" << std::endl;
                std::cerr << " Vs=" << Vs << std::endl;

                matrix<apz> aVs(NS,NS);
                const apz   dVs = apk::adjoint_gram(aVs,Vs);
                std::cerr << "aVs=" << aVs << std::endl;
                std::cerr << "dVs=" << dVs << std::endl;

                for(size_t j=1;j<=NS;++j)
                {
                    const Species &sp = *seeking[j];
                    const double   Cj = C[sp.indx];
                    if(Cj<0)
                    {
                        std::cerr << sp << " = " << Cj << std::endl;
                        Vector v(N,0);
                        for(size_t i=N;i>0;--i)
                        {
                            v[i] = -(Vs[j][i]*Cj) / Vs2[j];
                        }
                        std::cerr << "v" << j << "_full  = " << v << std::endl;
#if 0
                        guards.limit(v,C);
                        std::cerr << "v" << j << "_clip  = " << v << std::endl;
                        tao::mulset(v,2);
                        std::cerr << "v" << j << "_twice = " << v << std::endl;
                        guards.limit(v,C);
                        std::cerr << "v" << j << "_clip2 = " << v << std::endl;
#endif
                    }
                    
                }
                
                
                
                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


