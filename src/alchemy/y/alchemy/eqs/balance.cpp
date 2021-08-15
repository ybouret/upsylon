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

        void Reactor:: buildXi(const Accessible &C) throw()
        {
            assert(N>0);
            assert(NA>0);
            Addressable &Xi = aliasing::_(xi); assert(N==xi.size());
            
            for(size_t i=N;i>0;--i)
            {
                std::cerr << "for "; eqs.print(std::cerr,eqs(i)) << std::endl;
                Xi[i] = 0;
                xiTry.free();
                const accessible<unit_t> &nu_i = Nu[i];
                for(size_t j=M;j>0;--j)
                {
                    const unit_t nu = nu_i[j];
                    if(nu)
                    {
                        assert(active[j]);
                        const double c = C[j];
                        if(c<0)
                        {
                            xiTry.push_back_(-c/nu);
                        }
                    }
                }
                std::cerr << "xiTry=" << xiTry << std::endl;
                if(xiTry.size())
                {
                    hsort(xiTry, comparison::decreasing_abs<double>);
                    std::cerr << " => " << xiTry << std::endl;
                }
            }
            
            std::cerr << "Guess=" << Xi << std::endl;
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
               
               
                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


