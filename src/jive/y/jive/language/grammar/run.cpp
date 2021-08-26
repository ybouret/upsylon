#include "y/jive/language/grammar.hpp"
#include "y/ios/align.hpp"

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
                observer.free();
                XNode       *node = NULL;
                const Axiom *root = getRoot();
                if(!root) throw exception("%s has no root axiom",**name);

                //--------------------------------------------------------------
                //
                // main acceptance
                //
                //--------------------------------------------------------------
                if(Axiom::Verbose) std::cerr << "[" << name << "] root axiom : <" << root->name << ">" << std::endl;
                const bool res = root->accept(node,source,lexer,observer);
                XTree      tree( node );
                if(Axiom::Verbose) std::cerr << "[" << name << "] " << (res? Axiom::Accepted : Axiom::Rejected) << std::endl;
                
                
                //--------------------------------------------------------------
                //
                // analyse result
                //
                //--------------------------------------------------------------
                observer.free();
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
                const Lexemes &lexemes = *lexer;
                if(lexemes.size<=0)
                {
                    //----------------------------------------------------------
                    // empty source
                    //----------------------------------------------------------
                    throw exception("%s does not accept empty %s",**name, **(source.context().tag) );
                }
                else
                {
                    //----------------------------------------------------------
                    // find first non accepted lexeme
                    //----------------------------------------------------------
                    const Lexeme *curr = lexemes.head; assert(curr);
                    for(;curr;curr=curr->next)
                    {
                        if(NULL==curr->owner) goto FORMAT_EXCEPTION;
                    }
                    throw exception("%s is corrupted, all terminals were accepted from %s",**name, **(source.context().tag));

                FORMAT_EXCEPTION:
                    assert(curr);
                    assert(NULL==curr->owner);
                    exception     excp;
                    curr->stampTo(excp);
                    curr->writeOn(excp,*this);
                    const Lexeme *prev = curr->prev;
                    if(prev)
                    {
                        const Aggregate *before = prev->owner; assert(before);
                        const char      *prior  = **(before->name);
                        switch(prev->usage)
                        {
                            case Lexeme::Core:
                                excp.cat(" invalid syntax in  %s",prior);
                                break;

                            case Lexeme::Done:
                                excp.cat(" invalid syntax after %s",prior);
                                break;
                        }
                    }
                    else
                    {
                        excp.cat(" is invalid standalone");
                    }
                    excp.cat(" for %s", **name);
                    throw excp;
                }




            }


        }
    }
}
