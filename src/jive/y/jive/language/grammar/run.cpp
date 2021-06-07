#include "y/jive/language/grammar.hpp"
namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            

            XNode * Grammar:: run(Source &source,
                                  Lexer  &lexer)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const Axiom *root = getRoot();
                if(!root) throw exception("%s has no root Axiom",**name);
                XNode    *node = NULL;
                Observer  obs  = { 0 };

                //--------------------------------------------------------------
                //
                // main acceptance
                //
                //--------------------------------------------------------------
                Y_LANG_PRINTLN( "[" << name << "] root Axiom : <" << root->name << ">");
                const bool res = root->accept(node,source,lexer,obs);
                XTree      tree( node );
                Y_LANG_PRINTLN( "[" << name << "] " << (res? Axiom::Accepted : Axiom::Rejected) );
                
                if(res)
                {
                    if(tree.is_empty())
                    {
                        throw exception("invalid empty tree");
                    }
                    // error detection
                    Lexeme *next = lexer.get(source);
                    if(next)
                    {
                        lexer.unget(next);
                        exception excp;
                        next->stampTo(excp);
                        excp.cat("extraneous");
                        next->labelTo(excp);
                        if(next->size)
                        {
                            excp.cat(" ");
                            next->charsTo(excp);
                        }
                        throw excp;
                    }
                    
                    return Node::AST(tree.yield());
                    
                }
                else
                {
                    throw exception("failure");
                }

                
                
            }

        }
    }
}
