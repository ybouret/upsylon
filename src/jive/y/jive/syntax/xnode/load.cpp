
#include "y/jive/syntax/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace  Syntax
        {
            
            Node * Node:: Load(Module *module, const AxiomsDB &axioms)
            {
                Source       source(module);
                size_t       shift = 0;
                const string name = string::read(source, shift, "Axiom name");
                
                
                return NULL;
                
            }
            
        }
        
    }
    
}

