#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Chemical
    {
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
            Y_CHEMICAL_PRINTLN(" Cr=" << Cr);
            return res;
        }

        bool System:: balanceReplica(Addressable &C) throw()
        {
            static const size_t from =      2;
            static const size_t curr = from+2;
            static const size_t next = curr+2;

            if(Verbosity)
            {
                Library::Indent(std::cerr,from) << "<Balance Replica>" << std::endl;
                lib.display(std::cerr,C,curr) << std::endl;
            }
            bool   success = true;
            size_t currBad = replicaProbe(C);

            if(currBad>0)
            {
                //--------------------------------------------------------------
                //
                //
                // Initialize Look Up Algorithm
                //
                //
                //--------------------------------------------------------------
                success      = false;
                size_t cycle = 1;
                for(size_t i=NR;i>0;--i)
                {
                    tao::set(Vr[i],replica[i]->nu);
                }
                Ur.assign_transpose(Vr);

                while(true)
                {

                    if(Verbosity)
                    {
                        Library::Indent(std::cerr,curr) << "<Cycle #" << cycle << ">" << std::endl;
                        showPrimary(std::cerr,C,next);
                        showReplica(std::cerr,C,next);
                        Library::Indent(std::cerr,next) << "Vr = " << Vr << std::endl;
                    }

                    //----------------------------------------------------------
                    //
                    // compute Xr
                    //
                    //----------------------------------------------------------
                    tao::gram(V2,Vr);
                    if(!LU::build(V2))
                    {
                        if(Verbosity)  Library::Indent(std::cerr,next) << "[[ Singular Replica System ]]" << std::endl;
                        success = false;
                        goto END_CYCLE;
                    }
                    LU::solve(V2,Cr);
                    tao::mul(xr,Ur,Cr);
                    if(Verbosity)  Library::Indent(std::cerr,next) << "Xr = " << xr << std::endl;
                    


                END_CYCLE:
                    if(Verbosity)
                    {
                        Library::Indent(std::cerr,curr) << "<Cycle #" << cycle << "/>" << std::endl;
                    }

                    if(!success)
                        break;

                    exit(-1);

                }


                //--------------------------------------------------------------
                //
                //
                // End of Look Up Algorithm
                //
                //
                //--------------------------------------------------------------
            }
            
            
            if(Verbosity)
            {
                lib.display(std::cerr,C,curr)   << std::endl;
                Library::Indent(std::cerr,curr) << " ==> " << Outcome(success) << " <==" << std::endl;
                Library::Indent(std::cerr,from) << "<Balance Replica/>" << std::endl;
            }
            
            return false;
        }

    }
    
}
