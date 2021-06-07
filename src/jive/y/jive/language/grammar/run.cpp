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


            static inline const char *nameOfHost(const Axiom *ax) throw()
            {
                return ax ? **(ax->name) : "(nil)";
            }

            static inline const char *nameOfUnit(const Lexeme *lx) throw()
            {
                return lx ? **(lx->label) : "(nil)";
            }


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
                Observer  obs  = { 0, 0, 0, 0};

                //--------------------------------------------------------------
                //
                // main acceptance
                //
                //--------------------------------------------------------------
                Y_LANG_PRINTLN( "[" << name << "] root Axiom : <" << root->name << ">");
                const bool res = root->accept(node,source,lexer,obs);
                XTree      tree( node );
                Y_LANG_PRINTLN( "[" << name << "] " << (res? Axiom::Accepted : Axiom::Rejected) );
                Y_LANG_PRINTLN( "[" << name << "].host : " << nameOfHost(obs.lastHost) );
                Y_LANG_PRINTLN( "[" << name << "].unit : " << nameOfUnit(obs.lastUnit) );



                //--------------------------------------------------------------
                //
                // analyse result
                //
                //--------------------------------------------------------------
                return res ? onAccept(tree.yield(),source,lexer,obs) : onReject(tree.yield(),source,lexer,obs);

            }

            XNode *Grammar:: onAccept(XNode  *        node,
                                      Source         &source,
                                      Lexer          &lexer,
                                      const Observer &obs) const
            {
                const Grammar &self = *this;
                XTree tree(node);

                if(NULL==node)
                {
                    throw exception("%s returned an empty AST!",**name);
                }

                assert(NULL!=node);
                Lexeme *next = lexer.get(source);
                if(next)
                {
                    lexer.unget(next);
                    exception excp;
                    next->stampTo(excp);
                    excp.cat("%s has extraneous ",**name);
                    next->writeOn(excp,self);
                    if(obs.lastHost)
                    {
                        excp.cat(" after ");
                        if(obs.lastUnit)
                        {
                            obs.lastUnit->writeOn(excp,self);
                            excp.cat(" of ");
                        }
                        excp.cat(" <%s>", **(obs.lastHost->name));
                    }
                    throw excp;
                }


                return Node::AST(tree.yield());
            }

            XNode * Grammar:: onReject(XNode          *node,
                                       Source         &source,
                                       Lexer          &lexer,
                                       const Observer &obs) const
            {
                XTree tree(node);

                throw exception("%s was rejected",**name);
                return tree.yield();
            }


        }
    }
}
