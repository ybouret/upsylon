#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Chemical
    {
        bool System:: balanceReplica(Addressable &C) throw()
        {
            if(Verbosity)
            {
                Library::Indent(std::cerr,2) << "<Balance Replica>" << std::endl;
                lib.display(std::cerr,C,4) << std::endl;
            }
            
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            size_t cycle = 1;
            for(size_t i=NR;i>0;--i)
            {
                tao::set(Vr[i],replica[i]->nu);
            }
            Vt.assign_transpose(Vr);
            bool result = false;
            
            
            while(true)
            {
                if(Verbosity)
                {
                    Library::Indent(std::cerr,4) << "<Cycle #" << cycle << ">" << std::endl;
                    showPrimary(std::cerr,C,6);
                    showReplica(std::cerr,C,6);
                    std::cerr << "      Vr = " << Vr << std::endl;
                }
                
                
                
                
                
                if(Verbosity)
                {
                    Library::Indent(std::cerr,4) << "<Cycle #" << cycle << "/>" << std::endl;
                }
                if(result)
                {
                    break;
                }
            }
            
            
            if(Verbosity)
            {
                lib.display(std::cerr,C,4)   << std::endl;
                Library::Indent(std::cerr,4) << " ==> " << (result ? "Success" : "Failure") << " <==" << std::endl;
                Library::Indent(std::cerr,2) << "<Balance Replica/>" << std::endl;
            }
            
            return false;
        }

    }
    
}
