
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/core/temporary-value.hpp"

#define Y_JIVE_GRAMLN(MSG) do { if(Axiom::Verbose) { std::cerr << "[[" << name << "]] " << MSG << std::endl; } } while(false)

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {


            static inline
            void excpLexeme(exception           &excp,
                            const Lexeme        &lexeme,
                            const Terminal::Type type,
                            const bool           full)
            {
                const char  *id   = **lexeme.label;
                const char  *whom = **lexeme.tag;
                const int    line =   lexeme.line;
                const int    col  =   lexeme.column;
                if(full)
                {
                    excp.cat("%s:%d%d: ",whom,line,col);
                }
                switch(type)
                {
                    case Terminal::Standard: {
                        const string s = lexeme.toString();
                        excp.cat("<%s> = '%s'",id,*s);
                    } break;

                    default:
                        excp.cat("<%s>",id);
                        break;
                }
                
            }


            Terminal::Type Grammar:: lexemeType(const Lexeme &lexeme) const
            {
                const string   &key = *lexeme.label;
                Axiom * const * ppA = registry.search_by(key);
                if(!ppA)
                {
                    exception excp;
                    excpLexeme(excp,lexeme,Terminal::Standard,true);
                    excp.cat(" unregistered in %s grammar",**name);
                    throw excp;
                }
                else
                {
                    assert(*ppA);
                    const Axiom &axiom = **ppA;
                    return axiom.as<Terminal>().type;
                }
            }

            static inline
            void dispLexeme(const Tag    &name,
                            const char   *title,
                            const Lexeme *lx)
            {
                assert(title);
                if(lx)
                {
                    Y_JIVE_GRAMLN(title << "    = <" << lx->label << "> (= '" << *lx << "')" );
                }
                else
                {
                    Y_JIVE_GRAMLN(title << "    = (nil)");
                }
            }



            Node * Grammar:: run(Lexer &lexer, Source &source) const
            {
                //--------------------------------------------------------------
                //
                // get root
                //
                //--------------------------------------------------------------
                const Axiom *root = axioms.head;
                if(!root) throw exception("%s has no root Axiom!",**name);
                Y_JIVE_GRAMLN("root=<"<<root->name<<">");

                //--------------------------------------------------------------
                //
                // try to accept root
                //
                //--------------------------------------------------------------
                Node         *tree    = NULL;
                Observer      guess   = { NULL, NULL, NULL };
                if( root->accept(tree,lexer,source,guess,0) )
                {
                    return processSuccess(tree,guess,lexer,source);
                }
                else
                {
                    assert(NULL==tree);
                    processFailure(guess,lexer,source);
                    return NULL;
                }

            }



            
            void Grammar:: processFailure(const Observer &guess, Lexer &lexer, Source &source) const
            {
                Y_JIVE_GRAMLN("failure");
                const Lexeme    *last = guess.lexeme;
                if(last)
                {
                    // something was accepted
                    dispLexeme(name, "last",last);
                    exception excp;
                    excpLexeme(excp,*last,lexemeType(*last),true);
                    excp.cat(" unfinished");
                    const Aggregate *hold = guess.parent;
                    if(hold)
                    {
                        excp.cat(" in <%s>", **(hold->name) );
                    }
                    else
                    {
                        excp.cat(" in unknown...");
                    }
                    throw excp;
                }
                else
                {
                    // nothing was accepted
                    const Lexeme *next = lexer.next(source);
                    dispLexeme(name, "next", next);
                    
                }
                throw exception("Syntax Error");
            }
        }

    }

}

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Node * Grammar:: processSuccess(Node           *tree,
                                            const Observer &guess,
                                            Lexer          &lexer,
                                            Source         &source) const
            {
                Node::Pointer    node( tree );
                const Lexeme    *last = guess.lexeme;
                const Aggregate *hold = guess.parent;
                Y_JIVE_GRAMLN("success");
                dispLexeme(name,"last",last);
                if(hold)
                {
                    Y_JIVE_GRAMLN("from <" << hold->name << ">");
                }
                else
                {
                    Y_JIVE_GRAMLN("from anonymous axiom");
                }

                if(tree)
                {
                    //----------------------------------------------------------
                    //
                    // success with tree : check no extra lexeme!
                    //
                    //----------------------------------------------------------
                    const Lexeme *next = lexer.next(source);
                    if(next)
                    {
                        dispLexeme(name,"next",next);
                        exception excp;
                        excpLexeme(excp, *next, lexemeType(*next),true);
                        excp.cat(" is extraneous");
                        if(last)
                        {
                            excp.cat(" after ");
                            excpLexeme(excp,*last,lexemeType(*last),false);
                            if(hold)
                            {
                                excp.cat(" of <%s>",**(hold->name));
                            }
                            else
                            {
                                excp.cat(" of anonymous axiom");
                            }
                        }
                        excp.cat(" in %s",**name);
                        throw excp;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // success with no tree, invalid grammar, try to guess
                    //
                    //----------------------------------------------------------
                    exception excp;
                    if(last)
                    {
                        excpLexeme(excp, *last, lexemeType(*last),true);
                        excp.cat(" was accepted");
                    }
                    else
                    {
                        const Lexeme *next = lexer.next(source);
                        if(next)
                        {
                            excpLexeme(excp,*next,lexemeType(*next),true);
                            excp.cat(" was scanned");
                        }
                        else
                        {
                            const Context &ctx = source.context();
                            excp.cat("%s: is empty", **(ctx.tag) );
                        }
                    }
                    excp.cat(" => NULL tree in %s",**name);
                    throw excp;
                }



                return Node::AST( node.yield() );
            }
        }

    }

}

