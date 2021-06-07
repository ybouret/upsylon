#include "y/jive/language/grammar.hpp"
namespace upsylon
{
    namespace Jive
    {
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
                return res ? onAccept(tree.yield(), source, lexer) : onReject(tree.yield(),source,lexer);

            }

            XNode *Grammar:: onAccept(XNode  *node,
                                      Source &source,
                                      Lexer  &lexer) const
            {
                XTree tree(node);


                if(NULL==node)
                {
                    throw exception("%s returned an empty AST!",**name);
                }


                return Node::AST(tree.yield());
            }

            XNode * Grammar:: onReject(XNode *node, Source &source, Lexer &lexer) const
            {
                XTree tree(node);

                throw exception("%s was rejected",**name);
                return tree.yield();
            }


        }
    }
}
