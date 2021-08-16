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

        static inline double psi_der(double x) { return min_of(0.0,x);  }
        static inline long   psi_sec(double x) { return x<=0.0 ? 1 : 0; }

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
                vector<long> Dbad(M,0);
                for(size_t j=M;j>0;--j)
                {
                    Dbad[j] = 0;
                    Cbad[j] = 0;
                    if(active[j])
                    {
                        const double Cj = C[j];
                        if(Cj<0) ++ nbad;
                        Cbad[j] = psi_der(Cj);
                        Dbad[j] = psi_sec(Cj);
                    }
                }
                std::cerr  << "nbad=" << nbad << std::endl;
                lib.display(std::cerr << "Psi2=",Dbad)   << std::endl;
                lib.display(std::cerr << "Psi1=",Cbad)   << std::endl;
                Addressable &Xi = aliasing::_(xi);
                std::cerr << "Nu="   << Nu << std::endl;
                std::cerr << "Psi1=" << Cbad << std::endl;
                tao::mul(Xi,Nu,Cbad);
                std::cerr << "dPsi=" << Xi << std::endl;
                
               
                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


