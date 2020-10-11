
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
                
                Node   *tree = NULL;
                Lexeme *mind = NULL;
                if(root->accept(tree,lexer,source,mind,0))
                {
                    //----------------------------------------------------------
                    //
                    // check result
                    //
                    //----------------------------------------------------------
                    Node::Pointer keep( tree );
                    
                    if(mind)
                    {
                        std::cerr << "Last accepted : <" << mind->label << "> = '" << (*mind) << "'" << std::endl;
                    }
                    if(tree)
                    {
                        return Node::AST( keep.yield() );
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else
                {
                    assert(NULL==tree);
                    //----------------------------------------------------------
                    //
                    // check for error
                    //
                    //----------------------------------------------------------
                    if(mind)
                    {
                        std::cerr << "Last accepted : <" << mind->label << "> = '" << (*mind) << "'" << std::endl;
                    }
                    return NULL;
                }
             }

        }

    }

}

