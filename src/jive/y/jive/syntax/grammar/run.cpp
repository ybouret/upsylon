
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/core/temporary-value.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            Node * Grammar:: run(Lexer &lexer, Source &source) const
            {
                
                const Axiom *root = axioms.head;
                if(!root)
                {
                    throw exception("%s has no root Axiom!",**name);
                }
                Node *tree = NULL;
                if(root->accept(tree,lexer,source,0))
                {
                    Node::Pointer keep( tree );
                    
                    return keep.yield();;
                }
                else
                {
                    assert(NULL==tree);
                    return NULL;
                }
             }

        }

    }

}

