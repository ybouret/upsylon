
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
                else
                {
                    Cs[j] = 0;
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
                // info
                //
                //--------------------------------------------------------------
                if(Verbosity)
                {
                    lib.display(std::cerr << "    C1=" << std::endl,C,4) << std::endl;
                    std::cerr << "    <Conditions>" << std::endl;
                    for(size_t j=1;j<=NS;++j)
                    {
                        seeking[j]->display(std::cerr << "      ",C) << std::endl;
                    }
                    std::cerr << "    <Conditions/>" << std::endl;
                }

                //--------------------------------------------------------------
                //
                // check C
                //
                //--------------------------------------------------------------
                if( hasSeeking(C) )
                {
                    // first Rs
                    tao::set(Rs,Cs);

                    // initialize Vs
                    for(size_t j=NS;j>0;--j)
                    {
                        const Seeking &s = *seeking[j];
                        tao::set(Vs[j],s.nu);
                    }
                    VsT.assign_transpose(Vs);
                    Y_CHEMICAL_PRINTLN("    Vs   = " << Vs);

                    // compute IV2
                    tao::gram(IV2,Vs);
                    if(!LU::build(IV2))
                    {
                        std::cerr << "Singular system..." << std::endl;
                        return false;
                    }

                    // compute xs = Vs'*IV2*Rs
                    LU::solve(IV2,Rs);
                    tao::mul(xs,VsT,Rs);

                    Y_CHEMICAL_PRINTLN("    xs   = " << xs);

                    exit(-1);
                }


                
            }
            Y_CHEMICAL_PRINTLN("    [seeking balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Seeking/>");
            return result;
        }

    }

}

