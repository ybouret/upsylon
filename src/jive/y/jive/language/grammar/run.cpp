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
                xNode    *node = NULL;
                Observer  obs  = { 0 };

                // main acceptance
                Y_LANG_PRINTLN( "[" << name << "] root Axiom : <" << root->name << ">");
                const bool res = root->accept(node,source,lexer,obs);
                xTree      tree( node );
                Y_LANG_PRINTLN( "[" << name << "] " << (res? Axiom::Accepted : Axiom::Rejected) );
                
                if(res)
                {
                    // error detection
                    Lexeme *next = lexer.get(source);
                    if(next)
                    {
                        lexer.unget(next);
                        exception excp;
                        next->cat_stamp(excp);
                        excp.cat("extraneous");
                        next->cat_label(excp);
                        if(next->size)
                        {
                            excp.cat(" ");
                            next->cat_chars(excp);
                        }
                        throw excp;
                    }
                }
                else
                {
                    
                }


                return tree.yield();
            }

        }
    }
}
