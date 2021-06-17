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
                Observer  obs(name);

                //--------------------------------------------------------------
                //
                // main acceptance
                //
                //--------------------------------------------------------------
                if(Axiom::Verbose) std::cerr << "[" << name << "] root Axiom : <" << root->name << ">" << std::endl;
                const bool res = root->accept(node,source,lexer,obs);
                XTree      tree( node );
                if(Axiom::Verbose) std::cerr << "[" << name << "] " << (res? Axiom::Accepted : Axiom::Rejected) << std::endl;
                
                
                //--------------------------------------------------------------
                //
                // analyse result
                //
                //--------------------------------------------------------------
                if(res)
                {
                    return onAccept(tree.yield(),source,lexer);
                }
                else
                {
                    assert(NULL==node);
                    return onReject(source,lexer);
                }

            }

            XNode *Grammar:: onAccept(XNode  *        node,
                                      Source         &source,
                                      Lexer          &lexer) const
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                XTree          tree(node);
                const Grammar &self = *this;

                //--------------------------------------------------------------
                //
                // NULL node
                //
                //--------------------------------------------------------------
                if(NULL==node)
                {
                    throw exception("%s returned an empty syntax tree!",**name);
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
                    exception     excp;
                    next->stampTo(excp);
                    excp.cat("%s has extraneous ",**name);
                    next->writeOn(excp,self);
                    
                    const Axiom  *from = NULL;
                    const Lexeme *last = Node::LastLexeme(node,from);
                    if(last)
                    {
                        excp.cat(" after ");
                        last->writeOn(excp,self);
                        if(from)
                        {
                            excp.cat(" of <%s>", **(from->name) );
                        }
                    }
                    else
                    {
                        if(from)
                        {
                            excp.cat(" after <%s>", **(from->name) );
                        }
                    }
                    
                    
                    throw excp;
                }


                return Node::AST(tree.yield());
            }

            XNode * Grammar:: onReject(Source         &source,
                                       Lexer          &lexer) const
            {
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                
                std::cerr << "<Lexemes>" << std::endl;
                for(const Lexeme *lx = lexer.next(source); lx; lx=lx->next )
                {

                    std::cerr << " @" << lx->tag << ':' << lx->line << ':' << lx->column << ": ";
                    std::cerr << lx->label << " = '" << *lx << "'";
                    std::cerr << " <- " << (lx->owner ? **(lx->owner->name) : core::ptr::nil);
                    std::cerr << " usage: " << (lx->usageText());
                    std::cerr << std::endl;
                }
                std::cerr << "<Lexemes/>" << std::endl;

                //--------------------------------------------------------------
                // test source
                //--------------------------------------------------------------
                const Lexeme *curr = lexer.next(source);
                {
                    if(NULL==curr)
                    {
                        throw exception("%s does not accept an empty source",**name);
                    }
                }
                
                //--------------------------------------------------------------
                // format exception
                //--------------------------------------------------------------
                exception     excp;

                 
                throw excp;

            }


        }
    }
}
