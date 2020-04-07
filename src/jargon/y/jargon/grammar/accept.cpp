

#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
 
namespace upsylon {
    
    namespace Jargon {

        XNode * Grammar:: accept(Lexer &lexer, Source &source, const bool doAST) const
        {
            assert(ground);
            XNode       *xtree    = NULL;
            Y_JAXIOM(std::cerr << "[" << title << "] accepting..." << std::endl);
            
            if( ground->accept(xtree,lexer,source,xcache) )
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
                        lexeme->writeTo(excp,getTerminal(lexeme->label).isDefinite());
                        
                        const Lexeme *last = XNode::LastLexeme(xtree);
                        if(last)
                        {
                            excp.cat(" after");
                            last->writeTo(excp,getTerminal(last->label).isDefinite());
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
                Lexeme *lexeme = lexer.get(source);
                if(lexeme)
                {
                    lexer.unget(lexeme);
                    std::cerr << "with lexeme " << *lexeme << std::endl;
                }
                else
                {
                    std::cerr << "without lexeme" << std::endl;
                }
                
#if 0
                Lexeme *lexeme = lexer.get(source);
                if(lexeme)
                {
                    lexer.unget(lexeme);
                    exception excp("%s:%d:%d: [%s] illegal first",
                                   **(lexeme->tag),
                                   lexeme->line,
                                   lexeme->column,
                                   **title);
                    lexeme->writeTo(excp,getTerminal(lexeme->label).isDefinite());
                    throw excp;
                }
#endif
                throw exception("[%s] rejected <%s>...", **title, **(ground->label) );
            }
            
        }
        
    }
    
}

