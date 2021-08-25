#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"
#include "y/alea.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Alchemy
    {
        const char * Reactor:: Qualify(const double c) throw()
        {
            if(c<0)
            {
                return "(wrong)";
            }
            else
            {
                return "(valid)";
            }
        }
        
        
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
                
                if(NS>0)
                {
                    std::cerr << " Vs=" << Vs << std::endl;

                    matrix<apz> aVs(NS,NS);
                    const apz   dVs = apk::adjoint_gram(aVs,Vs);
                    std::cerr << "aVs=" << aVs << std::endl;
                    std::cerr << "dVs=" << dVs << std::endl;
                    matrix<apz> IVS(N,NS);
                    tao::mmul_ltrn(IVS, Vs, aVs);
                    std::cerr << "IVS=" << IVS << std::endl;

                    Vector Cs(NS,0);
                    for(size_t j=1;j<=NS;++j)
                    {
                        Cs[j] = -C[seeking[j]->indx];
                    }
                    std::cerr << "Cs=" << Cs << std::endl;
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


