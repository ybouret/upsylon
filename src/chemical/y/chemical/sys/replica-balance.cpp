#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/index.hpp"
#include "y/code/textual.hpp"
#include <iomanip>

namespace upsylon
{
    using namespace mkl;
    
    namespace Chemical
    {

        void   System:: replicaBuild() throw()
        {
#if 0
            for(size_t i=MR;i>0;--i)
            {
                tao::set(Vr[i],replica[i]->nu);
            }
            Ur.assign_transpose(Vr);
            tao::ld(go,true);
#endif
        }

        size_t System:: replicaProbe(const Accessible &C) throw()
        {
            size_t res = 0;
            for(size_t j=MR;j>0;--j)
            {
                const Replica &r = *replica[j];
                const Species &s = *r;
                const double   c = C[s.indx];
                if(c<0)
                {
                    Cr[j] = -c;
                    ++res;
                }
                else
                {
                    Cr[j] = 0;
                }
            }
            return res;
        }


        bool System:: replicaGuess() throw()
        {
#if 0
            tao::gram(V2,Vr);
            if(!LU::build(V2))
            {
                return false;
            }
            tao::set(Br,Cr);
            LU::solve(V2,Br);
            tao::mul(xr,Ur,Br);
            return true;
#endif
            return false;
        }

        void   System:: replicaJam(const size_t i) throw()
        {
#if 0
            go[i] = false;
            Vr.ld_col(i,0);
            Ur.ld_row(i,0);
#endif
        }
        
        

        bool System:: balanceReplica(Addressable &C) throw()
        {
            static const size_t from =      2;
            static const size_t curr = from+2;

            bool success = true;

            //------------------------------------------------------------------
            //
            // enter info
            //
            //------------------------------------------------------------------
            if(Verbosity) {
                Library::Indent(std::cerr,from) << "<Balance Replica>" << std::endl;
            }


            if(replicaProbe(C))
            {
                Vector xt(N,0);
                success = false;

            INIT:
                lib.display(std::cerr,C,curr) << std::endl;
                showPrimary(std::cerr,C,curr);
                showReplica(std::cerr,C,curr);

                std::cerr << "Cr=" << Cr << std::endl;
                tao::ld(go,false);
                for(size_t j=MR;j>0;--j)
                {
                    replica[j]->activate(go);
                }
            STEP:
                xv.free();
                for(size_t i=1;i<=N;++i)
                {
                    if(go[i]) xv.push_back_(i);
                }

                std::cerr << "go=" << go << std::endl;
                std::cerr << "xv=" << xv << std::endl;

                Matrix V2(MR,MR);
                size_t dim = xv.size();

                Matrix     Vr(MR,dim);
                Matrix     Ur(dim,MR);
                Vector     xr(dim,0);
                for(size_t j=MR;j>0;--j)
                {
                    const Replica     &repl = *replica[j];
                    const iAccessible &nutj = repl.nu;
                    for(size_t i=dim;i>0;--i)
                    {
                        Vr[j][i] = nutj[ xv[i] ];
                    }
                }
                Ur.assign_transpose(Vr);
                std::cerr << "Vr=" << Vr << std::endl;
                tao::gram(V2,Vr);
                if(!LU::build(V2))
                {
                    std::cerr << "Singular Matrix..." << std::endl;
                    goto DONE;
                }
                tao::set(Br,Cr);
                LU::solve(V2,Br);
                tao::mul(xr,Ur,Br);
                std::cerr << "xr=" << xr << std::endl;


                tao::ld(xi,0);
                for(size_t i=dim;i>0;--i)
                {
                    xi[ xv[i] ] = xr[i];
                }
                std::cerr << "xi=" << xi << std::endl;
                indexing::make(ix, comparison::decreasing_abs<double>,xi);
                std::cerr << "ix=" << ix << std::endl;

                for(size_t ii=N;ii>0;--ii)
                {
                    const size_t   i = ix[ii]; if(!go[i]) continue;
                    double        &x = xi[i];
                    const Primary &p = *primary[i];
                    if(x>0)
                    {
                         if(!p.queryForward(C))
                        {
                            std::cerr << "blocked forward " << *p << std::endl;
                            go[i] = false;
                            goto STEP;
                        }
                    }
                    else
                    {
                        if(x<0)
                        {
                            if(!p.queryReverse(C))
                            {
                                std::cerr << "blocked reverse " << *p << std::endl;
                                go[i] = false;
                                goto STEP;
                            }
                        }
                        else
                        {
                            x=0;
                        }
                    }
                }


                for(size_t ii=N;ii>0;--ii)
                {
                    const size_t   i = ix[ii]; if(!go[i])     continue;
                    double        &x = xi[i];  if(fabs(x)<=0) continue;
                    const Primary &p = *primary[i];
                    std::cerr << "Trying " << *p << std::endl;
                    if(p.xmove(C,x,xt))
                    {
                        std::cerr << "cut" << std::endl;
                    }
                    else
                    {
                        std::cerr << "full" << std::endl;
                    }
                }

                if(replicaProbe(C))
                {
                    goto INIT;
                }
                else
                {
                    success = true;
                    goto DONE;
                }

            }

        DONE:
            if(Verbosity) {
                lib.display(std::cerr,C,curr) << std::endl;
                Library::Indent(std::cerr,curr) << " ==> " << Outcome(success) << " <==" << std::endl;
                Library::Indent(std::cerr,from) << "<Balance Replica/>" << std::endl;
            }

            return success;

        }

#if 0
        bool System:: balanceReplica(Addressable &C) throw()
        {
            static const size_t from =      2;
            static const size_t curr = from+2;
            static const size_t next = curr+2;

            //------------------------------------------------------------------
            //
            //
            // Enter Algorithm
            //
            //
            //------------------------------------------------------------------
            bool    success = true;
            size_t  currBad = replicaProbe(C);
            if(Verbosity)
            {
                Library::Indent(std::cerr,from) << "<Balance Replica>" << std::endl;
            }


            if(currBad>0)
            {
                //--------------------------------------------------------------
                //
                //
                // initialize full matrix from replica
                //
                //
                //--------------------------------------------------------------
                success      = false;
                size_t cycle = 0;
            INITIALIZE:
                ++cycle;
                replicaBuild();
                if(Verbosity)
                {
                    Library::Indent(std::cerr,from) << "---- Cycle #" << cycle << " ----" << std::endl;
                    lib.display(std::cerr,C,curr) << "=C"  << std::endl;
                    showPrimary(std::cerr,C,curr);
                    showReplica(std::cerr,C,curr);
                    Library::Indent(std::cerr,curr) << "Cr = " << Cr << std::endl;
                }

                //--------------------------------------------------------------
                //
                //
                // compute step with dimension reduction
                //
                //
                //--------------------------------------------------------------
                {
                COMPUTE_STEP:
                    if(Verbosity)
                    {
                        Library::Indent(std::cerr,curr) << "Vr = " << Vr  << std::endl;
                        Library::Indent(std::cerr,curr) << "go = " << go  << std::endl;
                    }

                    //----------------------------------------------------------
                    //
                    // compute step from current Vr/Ur
                    //
                    //----------------------------------------------------------
                    if(!replicaGuess())
                    {
                        if(Verbosity) Library::Indent(std::cerr,next) << "[[ Singular Replica ]]" << std::endl;
                        goto DONE;
                    }

                    if(Verbosity) Library::Indent(std::cerr,curr) << "xr = " << xr    << std::endl;

                    //----------------------------------------------------------
                    //
                    // check if dimension need to be reduced
                    //
                    //----------------------------------------------------------
                    if( replicaJammedByPrimary(C) )
                    {
                        if(Verbosity) Library::Indent(std::cerr,curr) << "<Jam/>" << std::endl;
                        goto COMPUTE_STEP;
                    }
                    else
                    {
                        if(Verbosity) Library::Indent(std::cerr,curr) << "<Run/>" << std::endl;

                    }
                }

                //--------------------------------------------------------------
                //
                //
                // try to move full
                //
                //
                //--------------------------------------------------------------
                indexing::make(ix,comparison::decreasing_abs<double>,xr);
                if(Verbosity)
                {
                    Library::Indent(std::cerr,curr) << "ix = " << ix << std::endl;
                    for(size_t ii=N;ii>0;--ii)
                    {
                        const size_t       i  = ix[ii];
                        const Equilibrium &eq = **primary[i];
                        Library::Indent(std::cerr,curr);
                        if(go[i])
                            std::cerr << "(*) " << eq << " : " << std::setw(16) << xr[i];
                        else
                            std::cerr << "( ) " << eq;
                        std::cerr << std::endl;
                    }
                }

                replicaSolve(C,curr);
                const size_t nextBad = replicaProbe(C);
                if(nextBad>0)
                {
                    currBad = nextBad;
                    goto INITIALIZE;
                }
                success=true;
            }

            //------------------------------------------------------------------
            //
            //
            // Leave Algorithm
            //
            //
            //------------------------------------------------------------------
        DONE:
            if(Verbosity)
            {
                Library::Indent(std::cerr,curr) << " ==> " << Outcome(success) << " <==" << std::endl;
                Library::Indent(std::cerr,from) << "<Balance Replica/>" << std::endl;
            }

            return success;
        }
#endif

    }
    
}
