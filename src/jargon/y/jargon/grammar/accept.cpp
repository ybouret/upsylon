

#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
 
namespace upsylon {
    
    namespace Jargon {

        XNode * Grammar:: accept(Lexer &lexer, Source &source, const bool doAST) const
        {
            assert(ground);
            XNode       *xtree = NULL;
            Y_JAXIOM(std::cerr << "[" << title << "] accepting..." << std::endl);
            if( ground->Y_JARGON_AXIOM_ACCEPT(xtree) )
            {
                //--------------------------------------------------------------
                //
                // detecting errors
                //
                //--------------------------------------------------------------
                if(xtree==NULL)
                {
                    throw exception("[%s]  accepted <%s> a NULL tree", **title, **(ground->label) );
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
                        exception excp("%s:%d:%d: [%s] unexpected",
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
                std::cerr << "[Rejected!!]" << std::endl;
                std::cerr << "with #" << lexer.lexemes.size << std::endl;
                
                const Lexemes &analyzed = lexer.lexemes;
                if(analyzed.size<=0)
                {
                    throw exception("[%s] rejected empty input",**title);
                }
                else
                {
                    exception     excp("[%s] rejected ",**title);
                    const Lexeme *last = analyzed.tail;
                    last->writeTo(excp, isDefinite(last) );
                    if(last->prev)
                    {
                        last = last->prev;
                        excp.cat(" after ");
                        last->writeTo(excp, isDefinite(last) );
                    }
                   
                    throw excp;
                }
                

            }
            
        }
        
    }
    
}

