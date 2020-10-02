
#include "y/jive/syntax/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() throw()
            {}
            
            void Grammar:: add(Axiom *axiom)
            {
                assert(axiom!=NULL);
                axioms.push_back(axiom);
                try {
                    
                }
                catch(...)
                {
                    delete axioms.pop_back();
                    throw;
                }
            }

        }
        
    }
    
}


