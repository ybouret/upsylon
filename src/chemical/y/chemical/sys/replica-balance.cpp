#include "y/chemical/system.hpp"


namespace upsylon
{
    
    namespace Chemical
    {
        bool System:: balanceReplica(Addressable &C) throw()
        {
            if(Verbosity)
            {
                Library::Indent(std::cerr,2) << "<Balance Replica>" << std::endl;
                lib.display(std::cerr,C,4) << std::endl;
            }
            
            size_t cycle = 1;
            while(true)
            {
                if(Verbosity)
                {
                    Library::Indent(std::cerr,4) << "<Cycle #" << cycle << ">" << std::endl;
                    showPrimary(std::cerr,C,6);
                    showReplica(std::cerr,C,6);
                }
                
                if(Verbosity)
                {
                    Library::Indent(std::cerr,4) << "<Cycle #" << cycle << "/>" << std::endl;
                }
                break;
            }
            
            
            if(Verbosity)
            {
                lib.display(std::cerr,C,4)   << std::endl;
                //Library::Indent(std::cerr,4) << " ==> " << (result ? "Success" : "Failure") << " <==" << std::endl;
                Library::Indent(std::cerr,2) << "<Balance Replica/>" << std::endl;
            }
            
            return false;
        }

    }
    
}
