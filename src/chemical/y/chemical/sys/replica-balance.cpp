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
            for(size_t i=NR;i>0;--i)
            {
                tao::set(Vr[i],replica[i]->nu);
            }
            Ur.assign_transpose(Vr);
            tao::ld(go,true);
        }

        size_t System:: replicaProbe(const Accessible &C) throw()
        {
            size_t res = 0;
            for(size_t j=NR;j>0;--j)
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
            tao::gram(V2,Vr);
            if(!LU::build(V2))
            {
                return false;
            }
            tao::set(Br,Cr);
            LU::solve(V2,Br);
            tao::mul(xr,Ur,Br);
            return true;
        }

        void   System:: replicaJam(const size_t i) throw()
        {
            go[i] = false;
            Vr.ld_col(i,0);
            Ur.ld_row(i,0);
        }


        size_t System:: replicaJammedByPrimary(const Accessible &C) throw()
        {
            size_t jammed = 0;

            for(size_t i=N;i>0;--i)
            {
                const double x = xr[i];
                if(x>0)
                {
                    if( !primary[i]->queryForward(C) )
                    {
                        replicaJam(i);
                        ++jammed;
                    }
                }
                else
                {
                    if(x<0)
                    {
                        if(!primary[i]->queryReverse(C))
                        {
                            replicaJam(i);
                            ++jammed;
                        }
                    }
                }
            }
            return jammed;
        }


        void  System:: replicaSolve(Addressable &C, const size_t indent) throw()
        {
            const size_t sub = indent+2;
            if(Verbosity) Library::Indent(std::cerr,indent) << "<Replica Solve>" << std::endl;
            for(size_t ii=N;ii>0;--ii)
            {
                const size_t i = ix[ii];
                const double x = xr[i];
                primary[i]->move(C,x,xi);
                if(Verbosity)
                {
                    lib.display(std::cerr,C,sub) << std::endl;
                }
            }
            if(Verbosity) Library::Indent(std::cerr,indent) << "<Replica Solve/>" << std::endl;

        }

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
                lib.display(std::cerr,C,curr) << "=C"  << std::endl;
                showPrimary(std::cerr,C,curr);
                showReplica(std::cerr,C,curr);
                Library::Indent(std::cerr,curr) << "Cr = " << Cr << std::endl;
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
            INITIALIZE:
                replicaBuild();

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
                        Library::Indent(std::cerr,curr) << "<Jam/>" << std::endl;
                        goto COMPUTE_STEP;
                    }
                    else
                    {
                        Library::Indent(std::cerr,curr) << "<Run/>" << std::endl;

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

    }
    
}
