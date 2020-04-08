

#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
 
namespace upsylon {
    
    namespace Jargon {

        XNode * Grammar:: accept(Lexer &lexer, Source &source, const bool doAST) const
        {
            assert(ground);
            XNode       *xtree = NULL;
            const Axiom *guess = NULL;
            Y_JAXIOM(std::cerr << "[" << title << "] accepting..." << std::endl);
            const bool  ok  = ground->Y_JARGON_AXIOM_ACCEPT(xtree);
            const char *by  = guess ? **(guess->label) : NULL;
            Y_JAXIOM(std::cerr << "[" << title << "] ok = " << ok << std::endl);
            Y_JAXIOM(std::cerr << "[" << title << "] by = " << (by ? by : "none") << std::endl);

            if( ok )
            {
                //--------------------------------------------------------------
                //
                // detecting errors
                //
                //--------------------------------------------------------------
                if(xtree==NULL)
                {
                    exception excp("[%s] accepted <%s> a NULL tree",**title, **(ground->label));
                    if( by ) excp.cat(" of <%s>", by);
                    throw excp;
                }
                
                
                try
                {
                    //----------------------------------------------------------
                    //
                    // still some lexeme ?
                    //
                    //----------------------------------------------------------
                    Lexeme *lexeme = lexer.get(source);
                    if(lexeme)
                    {
                        lexer.unget(lexeme);
                        exception excp("%s:%d:%d: [%s] unexpected extraneous",
                                       **(lexeme->tag),
                                       lexeme->line,
                                       lexeme->column,
                                       **title);
                        lexeme->writeTo(excp,isDefinite(lexeme));
                        
                        const Lexeme *last = XNode::LastLexeme(xtree);
                        if(last)
                        {
                            excp.cat(" after");
                            last->writeTo(excp,isDefinite(lexeme));
                        }
                        if( by ) excp.cat(" of <%s>", by);
                        throw excp;
                    }
                                
                }
                catch(...)
                {
                    XNode::Release(xtree,xcache);
                    throw;
                }
                
                //--------------------------------------------------------------
                //
                // ok, everything seems good
                //
                //--------------------------------------------------------------
                Y_JAXIOM(std::cerr << "[" << title << "] returning AST" << std::endl);
                return doAST ? AST(xtree) : xtree;

            }
            else
            {
                //--------------------------------------------------------------
                //
                // rejected!
                //
                //--------------------------------------------------------------
                //std::cerr << "[Rejected!!]" << std::endl;
                //std::cerr << "with #" << lexer.lexemes.size << std::endl;
                
                const Lexemes &analyzed = lexer.lexemes;
                if(analyzed.size<=0)
                {
                    throw exception("[%s] rejected empty input",**title);
                }
                else
                {
                    exception     excp("[%s] rejected ",**title);
                    if(1==analyzed.size)
                    {
                        excp.cat("single ");
                    }
                    const Lexeme *last = analyzed.tail;
                    last->writeTo(excp,isDefinite(last));
                    
                    if(last->prev)
                    {
                        const Lexeme *prev = last->prev;
                        excp.cat(" after ");
                        prev->writeTo(excp, isDefinite(prev) );
                    }
                    
                    //TODO analyze which..
                    if( by ) excp.cat(" of <%s>", by);

                    throw excp;
                }
                

            }
            
        }
        
    }
    
}

