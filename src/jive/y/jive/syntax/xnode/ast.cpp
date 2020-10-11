

#include "y/jive/syntax/axiom/all.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Node * Node:: AST(Node *node)  throw()
            {
                assert(node);
                const Axiom   &axiom = node->axiom;
                const uint32_t uuid  = axiom.uuid;
                switch(node->kind)
                {
                    case IsInternal:
                        
                        break;
                        
                    case IsTerminal:
                        break;
                }
                return node;
            }

        }
        
    }
    
}


