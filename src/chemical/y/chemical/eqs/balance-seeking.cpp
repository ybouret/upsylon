
#include "y/chemical/reactor.hpp"
#include "y/code/textual.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/sort/index.hpp"
#include <iomanip>

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
                const double Cj = C[seeking[j]->sp.indx];
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


        bool Reactor:: computeXS() throw()
        {
            tao::gram(IV2,Vs);
            if(!LU::build(IV2))
            {
                Y_CHEMICAL_PRINTLN("    Singular System");
                return false;
            }
            tao::set(Rs,Cs);
            LU::solve(IV2,Rs);
            tao::mul(xs,VsT,Rs);
            Y_CHEMICAL_PRINTLN("    xs   = " << xs);
            return true;
        }

        void   Reactor:: jam(const size_t i) throw()
        {
            Vs.ld_col(i,0);
            VsT.ld_row(i,0);
        }
        
        size_t Reactor:: countJammed(const Accessible &C) throw()
        {
            size_t nj=0;
            tao::ld(ok,true);
            for(size_t i=N;i>0;--i)
            {
                const double x = xs[i];
                if(x>0.0)
                {
                    if(!leading[i]->queryForward(C))
                    {
                        ++nj;
                        jam(i);
                        ok[i] = false;
                    }
                }
                else
                {
                    if(x<0.0)
                    {
                        if(!leading[i]->queryReverse(C))
                        {
                            ++nj;
                            jam(i);
                            ok[i]=false;
                        }
                    }
                    else
                    {
                        // nothing...
                    }
                }
            }
            return nj;
        }


        bool Reactor:: moveFull(Addressable &C) throw()
        {
            bool result = true;

            for(size_t ii=N;ii>0;--ii)
            {
                const size_t i    = ix[ii]; if(!ok[i]) continue;
                const double x    = xs[i];
                const bool   move = fabs(x)>0;
                if(!move)
                {
                    Y_CHEMICAL_PRINTLN("    Forgetting " << leading[i]->root << ' ' << core::ptr::nil);
                    continue;
                }

                const Leading &l = *leading[i];
                Y_CHEMICAL_PRINTLN("    Processing " << l.root << ' ' << l.kindText() << " @" << x);

                tao::ld(xi,0);
                switch(l.kind)
                {
                    case Leading::LimitedByNone:
                        // move full
                        xi[i] = x;
                        tao::mul_add(C,NuT,xi);
                        l.ensurePositive(C);
                        break;

                    case Leading::LimitedByReac:
                        
                        break;

                    case Leading::LimitedByProd:
                        break;

                    case Leading::LimitedByBoth:
                        break;

                }


            }

            return result;
        }


        bool Reactor:: balanceSeeking(Addressable &C) throw()
        {
            Y_CHEMICAL_PRINTLN("  <Balance Seeking>");
            bool result = true;
            if(NS>0)
            {
                //--------------------------------------------------------------
                //
                // status ath this point
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
                    std::cerr << "    <Limits>" << std::endl;
                    for(size_t j=1;j<=N;++j)
                    {
                        leading[j]->display(std::cerr,C,6);
                    }
                    std::cerr << "    <Limits/>" << std::endl;

                }

                //--------------------------------------------------------------
                //
                // check C
                //
                //--------------------------------------------------------------
                if( hasSeeking(C) )
                {
                    //----------------------------------------------------------
                    //
                    // initialize full Vs
                    //
                    //----------------------------------------------------------
                    for(size_t j=NS;j>0;--j)
                    {
                        const Seeking &s = *seeking[j];
                        tao::set(Vs[j],s.nu);
                    }
                    VsT.assign_transpose(Vs);
                    Y_CHEMICAL_PRINTLN("    Vs   = " << Vs);
                    Y_CHEMICAL_PRINTLN("    VsT  = " << VsT);

                    //----------------------------------------------------------
                    //
                    // compute initial xs
                    //
                    //----------------------------------------------------------
                    if(!computeXS())
                    {
                        return false;
                    }

                    //----------------------------------------------------------
                    //
                    // check if we use all equilibira and recompute xs
                    //
                    //----------------------------------------------------------
                    const size_t jammed = countJammed(C);
                    if(jammed)
                    {
                        std::cerr << "#JAMMED=" << jammed << std::endl;
                        Y_CHEMICAL_PRINTLN("    Vs   = " << Vs);
                        Y_CHEMICAL_PRINTLN("    VsT  = " << VsT);
                        if(!computeXS())
                        {
                            return false;
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // Try and move now
                    //
                    //----------------------------------------------------------
                    indexing::make(ix,comparison::decreasing_abs<double>,xs);
                    std::cerr << ix << std::endl;
                    if(Verbosity)
                    {
                        const size_t N1 = N+1;
                        for(const ENode *node=eqs->head();node;node=node->next)
                        {
                            const Equilibrium &eq = ***node;
                            const size_t       i  = eq.indx;
                            Library::Indent(std::cerr,6) << eq;
                            std::cerr << " : " << (ok[i]? "active" : "jammed" );
                            std::cerr << " : #" << std::setw(3) << (N1-ix[i]);
                            std::cerr << " : " << xs[i];
                            std::cerr << std::endl;
                        }
                    }

                    moveFull(C);

                    


                    exit(-1);
                }


                
            }
            Y_CHEMICAL_PRINTLN("    [seeking balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Seeking/>");
            return result;
        }

    }

}

