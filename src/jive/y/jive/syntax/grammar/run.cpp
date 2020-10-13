
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
                // get status
                //
                //--------------------------------------------------------------
                Node         *tree    = NULL;
                Guess         guess   = { NULL  };
                const bool    success = root->accept(tree,lexer,source,guess,0);
                Node::Pointer xnode(tree) ;

                Y_JIVE_GRAMLN("success = " << success);
                Y_JIVE_GRAMLN("tree    = " << (tree!=NULL) );
                const Lexeme *last = guess.lexeme;
                dispLexeme(name,"last",last);




                //--------------------------------------------------------------
                //
                // process status
                //
                //--------------------------------------------------------------
                if(success)
                {
                    if(tree)
                    {
                        const Lexeme *next = lexer.next(source);
                        if(next)
                        {
                            exception excp;
                            excpLexeme(excp, *next, lexemeType(*next),true);
                            excp.cat(" is extraneous");
                            if(last)
                            {
                                excp.cat(" after ");
                                excpLexeme(excp,*last,lexemeType(*last),false);
                            }
                            throw excp;
                        }
                        return Node::AST(xnode.yield());
                    }
                    else
                    {
                        //------------------------------------------------------
                        // invalid null tree
                        //------------------------------------------------------
                        return NULL;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // syntax error
                    //----------------------------------------------------------
                    std::cerr << "SYNTAX ERROR" << std::endl;
                    return NULL;
                }


            }

        }

    }

}

