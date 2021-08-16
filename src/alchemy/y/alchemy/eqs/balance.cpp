#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"

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
                    const Guard::State st = guards[eq.indx]->solve(C,NuT,Xi);
                    eqs.print(std::cerr << "guard ",eq) << std::endl;
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
            if(N>0)
            {
                assert(NA>0);
                showConditions(std::cerr,C);

                if( !balance1(C) )
                {
                    std::cerr << "Unbalanced Leading..." << std::endl;
                    return false;
                }
                std::cerr << "Balanced Leading!" << std::endl;
                
                size_t nbad = 0;
                for(size_t j=M;j>0;--j)
                {
                    Ibad[j] = false;
                    Cbad[j] = 0;
                    if(active[j])
                    {
                        const double Cj = C[j];
                        if(Cj<0)
                        {
                            ++nbad;
                            Ibad[j] = true;
                            Cbad[j] = -Cj;
                        }
                    }
                }
                std::cerr  << "nbad=" << nbad << std::endl;
                lib.display(std::cerr,Ibad)   << std::endl;
                lib.display(std::cerr,Cbad)   << std::endl;
                Addressable &Xi = aliasing::_(xi); assert(N==xi.size());
                
                if(nbad>0)
                {
                    tao::mul(Xi,Nu,Cbad);
                    std::cerr << "nub=" << Xi << std::endl;
                    std::cerr << "Nu=" << Nu << std::endl;
                    std::cerr << "Om=Nu*diagm(" << Ibad << ")*Nu'" << std::endl;
                    
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


