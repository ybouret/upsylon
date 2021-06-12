#include "y/jive/language/grammar.hpp"
namespace upsylon
{
    namespace Jive
    {

        namespace Lexical
        {
            exception     & Unit:: writeOn(exception &e, const Language::Grammar &G)   const throw()
            {
                writeTo(e,G.isStandard(this));
                return e;
            }

        }
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
                
                
                //--------------------------------------------------------------
                //
                // analyse result
                //
                //--------------------------------------------------------------
                if(res)
                {
                    return onAccept(tree.yield(),source,lexer,obs);
                }
                else
                {
                    assert(NULL==node);
                    return onReject(source,lexer,obs);
                }

            }

            XNode *Grammar:: onAccept(XNode  *        node,
                                      Source         &source,
                                      Lexer          &lexer,
                                      const Observer &obs) const
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const Grammar &self = *this;
                XTree tree(node);

                //--------------------------------------------------------------
                //
                // NULL node
                //
                //--------------------------------------------------------------
                if(NULL==node)
                {
                    throw exception("%s returned an empty AST!",**name);
                }

                //--------------------------------------------------------------
                //
                // valid node: check no extraneous
                //
                //--------------------------------------------------------------
                assert(NULL!=node);
                const Lexeme *next = lexer.next(source);
                if(next)
                {
                    exception excp;
                    next->stampTo(excp);
                    excp.cat("%s has extraneous ",**name);
                    next->writeOn(excp,self);
                    
                    
                    throw excp;
                }


                return Node::AST(tree.yield());
            }

            XNode * Grammar:: onReject(Source         &source,
                                       Lexer          &lexer,
                                       const Observer &obs) const
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                const Grammar &self = *this;

                //--------------------------------------------------------------
                // test source
                //--------------------------------------------------------------
                {
                    const Lexeme *next = lexer.next(source);
                    if(NULL==next)
                    {
                        throw exception("%s does not accept an empty source",**name);
                    }
                    assert(NULL!=lexer.last());
                }
                
                //--------------------------------------------------------------
                // format exception
                //--------------------------------------------------------------
                const Lexeme *last = lexer.last(); assert(last);
                exception     excp;
                last->stampTo(excp);
                excp.cat(" %s syntax error", **name);
                
                
                last->writeOn(excp,self);
                 
                throw excp;

            }


        }
    }
}
