
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

        const char * Reactor:: StateText(const State s) throw()
        {
            switch(s)
            {
                case Jam : return "Jam";
                case Nil : return "Nil";
                case All : return "All";
                case Cut : return "Cut";
            }
            return "???";
        }


        bool Reactor:: seekingQuery(const Accessible &C) throw()
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

        bool Reactor:: seekingExtra() throw()
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

        void   Reactor:: seekingBuild() throw()
        {
            tao::ld(ok,true);
            for(size_t j=NS;j>0;--j)
            {
                const Seeking &s = *seeking[j];
                tao::set(Vs[j],s.nu);
            }
            VsT.assign_transpose(Vs);
            Y_CHEMICAL_PRINTLN("    Vs   = " << Vs);
            Y_CHEMICAL_PRINTLN("    VsT  = " << VsT);
        }

        bool   Reactor:: seekingRaise(const Accessible &C) throw()
        {
        FIND_XS:
            //----------------------------------------------------------
            //
            // compute initial xs
            //
            //----------------------------------------------------------
            if(!seekingExtra())
            {
                return false;
            }

            //----------------------------------------------------------
            //
            // check if we use all equilibira and recompute xs
            //
            //----------------------------------------------------------
            const size_t jammed = seekingJammd(C);
            if(jammed)
            {
                Y_CHEMICAL_PRINTLN("    Vs   = " << Vs);
                Y_CHEMICAL_PRINTLN("    VsT  = " << VsT);
                goto FIND_XS;
            }

            return true;
        }

        

        
        void   Reactor:: seekingBlock(const size_t i) throw()
        {
            Vs.ld_col(i,0);
            VsT.ld_row(i,0);
        }
        
        size_t Reactor:: seekingJammd(const Accessible &C) throw()
        {
            size_t nj=0;
            Y_CHEMICAL_PRINTLN("    <Jamming>");
            for(size_t i=N;i>0;--i)
            {
                if(!ok[i]) continue; //! already jammed
                const double x = xs[i];
                if(x>0.0)
                {
                    const Leading &l = *leading[i];
                    if(!l.queryForward(C))
                    {
                        ++nj;
                        seekingBlock(i);
                        ok[i] = false;
                        Y_CHEMICAL_PRINTLN("      no " << l.root << " forward");
                    }
                }
                else
                {
                    if(x<0.0)
                    {
                        const Leading &l = *leading[i];
                        if(!l.queryReverse(C))
                        {
                            ++nj;
                            seekingBlock(i);
                            ok[i]=false;
                            Y_CHEMICAL_PRINTLN("      no " << l.root << " reverse");
                        }
                    }
                    else
                    {
                        // nothing...
                    }
                }
            }
            Y_CHEMICAL_PRINTLN("    <Jamming/>");
            return nj;
        }


        void Reactor:: seekingIndex() throw()
        {
            indexing::make(ix,comparison::decreasing_abs<double>,xs);

            if(Verbosity)
            {
                std::cerr << "    ix   = " << ix << std::endl;
                const size_t N1 = N+1;
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    const size_t       i  = eq.indx;
                    Library::Indent(std::cerr,6) << eq;
                    std::cerr << " : "  << (ok[i]? "active" : "jammed" );
                    std::cerr << " : #" << std::setw(3) << (N1-ix[i]);
                    std::cerr << " : "  << xs[i];
                    std::cerr << std::endl;
                }
            }
        }
        
        bool Reactor:: seekingSolve(Addressable &C) throw()
        {
            bool          result = true;
            vector<State> stype(N,Jam);

            for(size_t ii=N;ii>0;--ii)
            {
                //--------------------------------------------------------------
                // is it active ?
                //--------------------------------------------------------------
                const size_t i    = ix[ii];
                if(!ok[i])
                {
                    Y_CHEMICAL_PRINTLN("      Discarding " << leading[i]->root << " (jammed)");
                    continue;
                }
                
                //--------------------------------------------------------------
                // is it not nul
                //--------------------------------------------------------------
                const double x    = xs[i];
                const bool   move = fabs(x)>0;
                if(!move)
                {
                    stype[i] = Nil;
                    Y_CHEMICAL_PRINTLN("      Forgetting " << leading[i]->root << ' ' << core::ptr::nil);
                    continue;
                }
                
                //--------------------------------------------------------------
                // let's try
                //--------------------------------------------------------------
                const Leading &l = *leading[i];
                Y_CHEMICAL_PRINTLN("      Processing " << l.root << ' ' << l.kindText() << " @" << x);

                stype[i] = All;
                if(!l.tryMoveFull(x,C,xi))
                {
                    stype[i] = Cut;
                    result = false;
                }
                
                
            }

            if(Verbosity)
            {
                for(size_t i=1;i<=N;++i)
                {
                    std::cerr << "      " << leading[i]->root << " : " << StateText(stype[i]) << std::endl;
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
                // check invalid C and take action
                //
                //--------------------------------------------------------------
                if( seekingQuery(C) )
                {
                    //----------------------------------------------------------
                    //
                    // initialize full Vs
                    //
                    //----------------------------------------------------------
                    seekingBuild();
                    
                    //----------------------------------------------------------
                    //
                    // find dimension(s)
                    //
                    //----------------------------------------------------------
                    if(!seekingRaise(C)) return false;

                    //----------------------------------------------------------
                    //
                    // indexing
                    //
                    //----------------------------------------------------------
                    seekingIndex();

                    //----------------------------------------------------------
                    //
                    // Try and move now
                    //
                    //----------------------------------------------------------
                    Y_CHEMICAL_PRINTLN("    <Moving>");
                    const bool fullyMoved = seekingSolve(C);
                    lib.display(std::cerr,C,6)<<std::endl;
                    Y_CHEMICAL_PRINTLN("    <Moving/>");
                    if(fullyMoved)
                    {
                        Y_CHEMICAL_PRINTLN("  Full Moved");
                    }
                    else
                    {
                        Y_CHEMICAL_PRINTLN("  Part Moved");
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

