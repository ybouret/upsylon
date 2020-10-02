
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

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
                    if(!registry.insert_by(*(axiom->name),axiom))
                    {
                        throw exception("%s: multiple [%s]",**name, **(axiom->name));
                    }
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


