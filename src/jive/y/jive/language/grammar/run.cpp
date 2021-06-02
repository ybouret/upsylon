#include "y/jive/language/grammar.hpp"
namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            

            xNode * Grammar:: run(Source &source,
                                  Lexer  &lexer)
            {
                const Axiom *root = getRoot();
                if(!root) throw exception("%s has no root Axiom",**name);
                xNode    *tree = NULL;
                Observer  obs  = { 0 };

                Y_LANG_PRINTLN(name << ".root=<" << root->name << ">");
                if( !root->accept(tree,source,lexer,obs))
                {
                    // did not accept
                    Y_LANG_PRINTLN(name << " [rejected]");
                    return NULL;
                }


                return tree;
            }

        }
    }
}
