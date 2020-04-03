

#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
 
namespace upsylon {
    
    namespace Jargon {

        XNode * Grammar:: accept(Lexer &lexer, Source &source) const
        {
            assert(ground);
            XNode       *xtree    = NULL;
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
                    const Lexeme *lexeme = lexer.peek();
                    if(lexeme)
                    {
                        const bool definite = getTerminal(lexeme->label).isDefinite();
                        exception excp("%s:%d:%d: [%s] unexpected",
                                       **(lexeme->tag),
                                       lexeme->line,
                                       lexeme->column,
                                       **title);
                        lexeme->writeTo(excp,definite);
                        
                        const Lexeme *last = XNode::LastLexeme(xtree);
                        if(last)
                        {
                            excp.cat(" after");
                            last->writeTo(excp,definite);
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
                
                
                return xtree;

            }
            else
            {
                throw exception("[%s] accept <%s> failure...", **title, **(ground->label) );
            }
            
        }
        
    }
    
}

