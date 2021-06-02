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
                // initialize
                const Axiom *root = getRoot();
                if(!root) throw exception("%s has no root Axiom",**name);
                xNode    *tree = NULL;
                Observer  obs  = { 0 };

                Y_LANG_PRINTLN( "[" << name << "] root Axiom : <" << root->name << ">");
                const bool res = root->accept(tree,source,lexer,obs);
                Y_LANG_PRINTLN( "[" << name << "] " << (res? Axiom::Accepted : Axiom::Rejected) );



                return tree;
            }

        }
    }
}
