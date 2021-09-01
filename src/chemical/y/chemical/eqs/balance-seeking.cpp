
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
                // initialize full search
                //
                //--------------------------------------------------------------
                if(Verbosity)
                {
                    lib.display(std::cerr << "    C1=" << std::endl,C,4) << std::endl;
                }

                for(size_t j=NS;j>0;--j)
                {
                    const Seeking &s = *seeking[j];
                    tao::set(NuS[j],s.nu);
                    if(Verbosity) s.display(std::cerr << "    ",C) << std::endl;
                }
                NuST.assign_transpose(NuS);
                Y_CHEMICAL_PRINTLN("    NuS=" << NuS);
                
            }
            Y_CHEMICAL_PRINTLN("    [seeking balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Seeking/>");
            return result;
        }

#if 0
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
                    lib.display(std::cerr << "    C1=" << std::endl,C,4) << std::endl;
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
                        Y_CHEMICAL_PRINTLN("      ==> Singular Seeking Condition");
                        break;
                    }
                    LU::solve(NuS2,Cs);
                    tao::mul(xs,NuST,Cs);

                    if(Verbosity)
                    {
                        eqs.display(std::cerr << "    xs   =" << std::endl,xs,4) << std::endl;
                    }

                    // move Leading
                    Y_CHEMICAL_PRINTLN("    <Moving Procedure>" << std::endl);

                    for(size_t i=1;i<=N;++i)
                    {
                        const Leading &lead = *leading[i];
                        const bool     full = lead.moveAll(xs[i],C,NuT,xi);
                        if(!full)
                        {
                            NuS.  ld_col(i,0);
                            NuST. ld_row(i,0);
                        }
                    }
                    lib.display(std::cerr << "    Cm=" << std::endl,C,4) << std::endl;
                    Y_CHEMICAL_PRINTLN("    <Moving Procedure/>");
                }
            }

            Y_CHEMICAL_PRINTLN("    [seeking balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Seeking/>");
            return result;
        }
#endif

    }

}

