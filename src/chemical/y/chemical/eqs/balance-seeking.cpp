
#include "y/chemical/reactor.hpp"
#include "y/code/textual.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        bool Reactor:: hasSeeking(const Accessible &C) throw()
        {
            size_t nbad = 0;
            for(size_t j=NS;j>0;--j)
            {
                const double   Cj = C[seeking[j]->sp.indx];
                if(Cj<0)
                {
                    ++nbad;
                    Cs[j] = -Cj;
                }
            }
            Y_CHEMICAL_PRINTLN("    Cs   = " << Cs);
            return nbad>0;
        }

        bool Reactor:: balanceSeeking(Addressable &C) throw()
        {
            Y_CHEMICAL_PRINTLN("  <Balance Seeking>");
            bool result = true;
            if(NS>0)
            {

                //--------------------------------------------------------------
                //
                // initialize full search
                //
                //--------------------------------------------------------------
                if(Verbosity)
                {
                    lib.display(std::cerr << "C1=",C) << std::endl;
                }
                for(size_t j=NS;j>0;--j)
                {
                    tao::set(NuS[j],seeking[j]->nu);
                }
                NuST.assign_transpose(NuS);
                
                while( hasSeeking(C) )
                {
                    // compute optimized extent
                    tao::gram(NuS2,NuS);
                    Y_CHEMICAL_PRINTLN("    NuS  = " << NuS);
                    Y_CHEMICAL_PRINTLN("    NuS2 = " << NuS2);
                    if(!LU::build(NuS2))
                    {
                        result=false;
                        break;
                    }
                    LU::solve(NuS2,Cs);
                    tao::mul(xs,NuST,Cs);
                    
                    eqs.display(std::cerr << "xs=",xs) << std::endl;

                    // explore leading
                    for(size_t i=1;i<=N;++i)
                    {
                        (void) leading[i]->moveAll(xs[i],C,NuT,xi,lib);
                    }



                    exit(-1);
                }


            }

            Y_CHEMICAL_PRINTLN("    [seeking balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Seeking/>");
            return result;
        }

    }

}

