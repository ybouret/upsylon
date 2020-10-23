
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/core/temporary-value.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

#define Y_JIVE_GRAMLN(MSG) do { if(Axiom::Verbose) { std::cerr << "[[" << name << "]] " << MSG << std::endl; } } while(false)



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
                    excp.cat(" unregistered in %s",**name);
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
                    throw exception("SYNTAX ERROR");
                    return NULL;
                }

            }

            Node * Grammar:: processSuccess(Node           *tree,
                                            const Observer &guess,
                                            Lexer          &lexer,
                                            Source         &source) const
            {
                Node::Pointer xnode( tree );
                Y_JIVE_GRAMLN("success");
                Y_JIVE_GRAMLN("tree    = " << (tree!=NULL) );
                dispLexeme(name,"last",guess.lexeme);
                if(guess.parent)
                {
                    Y_JIVE_GRAMLN("hold    = " << guess.parent->name );
                }
                else
                {
                    Y_JIVE_GRAMLN("no parent!");
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
                        if(guess.lexeme)
                        {
                            const Lexeme &last = *guess.lexeme;
                            excp.cat(" after ");
                            excpLexeme(excp,last,lexemeType(last),false);
                        }
                        throw excp;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // success with no tree, invalid grammar
                    //
                    //----------------------------------------------------------

                    throw exception("NULL tree");
                }



                return Node::AST( xnode.yield() );
            }

        }

    }

}

