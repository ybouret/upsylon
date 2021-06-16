


#include "y/jive/language/axiom/wildcard/option.hpp"


namespace upsylon
{
    
    namespace Jive
    {
        
        namespace Language
        {
            Option:: ~Option() throw()
            {
            }
            
            Y_LANG_AXIOM_IMPL(Option)
            {
                Y_LANG_PRINTLN(obs.indent() << "|_" << name );
                bool                  found = false;
                {
                    const Observer::Scope keep(obs);
                    Node                 *node = NULL;
                    if(axiom.accept(node, source, lexer, obs))
                    {
                        if(node)
                        {
                            Node::Grow(tree,node);
                        }
                        found = true;
                    }
                    else
                    {
                        assert(NULL==node);
                    }
                }
                
                Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Status(found) << "]" );

                
                
                return true;
            }
            
        }
        
    }
    
}

