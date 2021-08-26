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


