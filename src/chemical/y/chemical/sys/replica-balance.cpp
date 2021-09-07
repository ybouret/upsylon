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
            if(Verbosity)
            {
                Library::Indent(std::cerr,2) << "<Balance Replica>" << std::endl;
                lib.display(std::cerr,C,4) << std::endl;
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
                        Library::Indent(std::cerr,4) << "<Cycle #" << cycle << ">" << std::endl;
                        showPrimary(std::cerr,C,6);
                        showReplica(std::cerr,C,6);
                        std::cerr << "      Vr = " << Vr << std::endl;
                    }

                    //----------------------------------------------------------
                    //
                    // compute Xr
                    //
                    //----------------------------------------------------------
                    tao::gram(V2,Vr);
                    if(!LU::build(V2))
                    {
                    }


                    if(Verbosity)
                    {
                        Library::Indent(std::cerr,4) << "<Cycle #" << cycle << "/>" << std::endl;
                    }

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
                lib.display(std::cerr,C,4)   << std::endl;
                Library::Indent(std::cerr,4) << " ==> " << Outcome(success) << " <==" << std::endl;
                Library::Indent(std::cerr,2) << "<Balance Replica/>" << std::endl;
            }
            
            return false;
        }

    }
    
}
