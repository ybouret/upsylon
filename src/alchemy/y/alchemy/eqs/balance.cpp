#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"
#include "y/alea.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Alchemy
    {
       
        
        
        bool Reactor:: balanceLeading(Addressable &C) throw()
        {
            assert(N>0);
            assert(NA>0);
            
            if(Verbosity)
            {
                std::cerr << "<Balance Leading>" << std::endl;
                lib.display(std::cerr << "C=", C) << std::endl;
                showConditions(std::cerr,C);
            }

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            Flags &OK = aliasing::_(ok);
            tao::ld(OK,true);
            bool balanced = true;

            {
                Y_ALCHEM_PRINTLN("  <Guard>");
                Addressable &Xi = aliasing::_(xi); assert(N==xi.size());
                for(const Equilibrium::Node *node = eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    if (Verbosity) eqs.print(std::cerr << "    ",eq) << std::endl;
                    const Guard::State st = guards[eq.indx]->solve(C,NuT,Xi,OK);
                    if (Verbosity) eqs.print(std::cerr << "    ",eq) << ' ' << Guard::StateText(st) << std::endl;
                    if(Guard::IsJammed==st)
                    {
                        balanced = false;
                    }
                    if(Verbosity&&node->next) std::cerr << std::endl;
                }
                Y_ALCHEM_PRINTLN("  <Guard/>");
            }

            if(Verbosity)
            {
                std::cerr << "  ==> [" <<textual::boolean(balanced) << "] <==" << std::endl;
                lib.display(std::cerr << "OK=",ok) << std::endl;
                lib.display(std::cerr << "C=",  C) << std::endl;
                std::cerr << "<Balance Leading/>"  << std::endl;
            }
            return balanced;
        }
        
        
        bool Reactor:: balance(Addressable &C) throw()
        {
            if(N>0)
            {
                assert(NA>0);
                //______________________________________________________________
                //
                // starting point: need to be reduced
                // _____________________________________________________________
                if( !balanceLeading(C) )
                {
                    return false;
                }

                //______________________________________________________________
                //
                // reduced starting point
                // _____________________________________________________________
                //showConditions(std::cerr,C);
                return false;

                if(NS>0)
                {
                    std::cerr << " Vs=" << Vs << std::endl;
                    matrix<unit_t> IVS(N,NS);
                    unit_t         dVs = 0;
                    {
                        matrix<apz> aVs(NS,NS);
                        const apz   dVs_ = apk::adjoint_gram(aVs,Vs);
                        dVs = dVs_.cast_to<unit_t>();
                        std::cerr << "aVs=" << aVs << std::endl;
                        std::cerr << "dVs=" << dVs << std::endl;
                        matrix<apz> IVS_(N,NS);
                        tao::mmul_ltrn(IVS_, Vs, aVs);
                        apk::convert(IVS,IVS_);
                        std::cerr << "IVS=" << IVS << std::endl;
                    }


                    Vector Cs(NS,0);
                    for(size_t j=1;j<=NS;++j)
                    {
                        const double Cj = C[seeking[j]->indx];
                        if(Cj<0)
                        {
                            Cs[j] = -Cj;
                        }
                    }
                    std::cerr << "Cs=" << Cs << std::endl;
                    Addressable &Xi = aliasing::_(xi);
                    tao::mul(Xi, IVS, Cs);
                    tao::divset(Xi,dVs,Xi);
                    std::cerr << "Xi0=" << Xi << std::endl;
                    guards.limit(Xi,C);
                    std::cerr << "Xi1=" << Xi << std::endl;

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


