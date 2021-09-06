
#include "y/chemical/system.hpp"


namespace upsylon
{
    
    namespace Chemical
    {
        
        bool System:: balancePrimary(Addressable &C) throw()
        {
            if(Verbosity)
            {
                Library::Indent(std::cerr,2) << "<Balance Primary>" << std::endl;
                lib.display(std::cerr,C,4) << std::endl;
                Library::Indent(std::cerr,4) << "<Solve Primary>" << std::endl;
            }
            
            bool result = true;
            for(size_t i=1;i<=N;++i)
            {
                if( false == (ok[i]=primary[i]->solve(C,xi)))
                {
                    result=false;
                }
                if(Verbosity&&i<N) std::cerr << std::endl;
            }
            
            
            if(Verbosity)
            {
                Library::Indent(std::cerr,4) << "<Solve Primary/>" << std::endl;
                lib.display(std::cerr,C,4)   << std::endl;
                eqs.display(std::cerr,ok,4)  << std::endl;
                Library::Indent(std::cerr,4) << " ==> " << (result ? "Success" : "Failure") << " <==" << std::endl;
                Library::Indent(std::cerr,2) << "<Balance Primary/>" << std::endl;
            }
            return result;
        }
        
        
    }
}
