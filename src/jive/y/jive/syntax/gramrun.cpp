
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

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
                Y_JIVE_PRINTLN(name << " run with root=<" << root->name << ">");
                Node *tree = NULL;
                if(root->accept(tree,lexer,source))
                {
                    
                    return tree;
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

