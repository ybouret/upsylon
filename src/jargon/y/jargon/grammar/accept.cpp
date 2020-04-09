

#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static inline void writeLexeme(exception     &excp,
                                       const Grammar &G,
                                       const Lexeme  *lexeme)
        {
            const Tag      &label = lexeme->label;
            const Terminal *term  = G.toTerminal(label);
            if( term )
            {
                lexeme->writeTo(excp, term->isDefinite() );
            }
            else
            {
                lexeme->writeTo(excp,false);
            }
        }
        
        static inline exception onError(const char   *which,
                                        const Grammar &G,
                                        const Lexeme *lexeme,
                                        const Axiom  *guess)
        {
            assert(which);
            assert(lexeme);
            exception       excp("[%s] %s ", ** G.title, which);
            writeLexeme(excp,G,lexeme);
            if(lexeme->prev)
            {
                excp.cat( " after ");
                writeLexeme(excp,G,lexeme->prev);
            }
            return excp;
        }
        
        XNode * Grammar:: accept(Lexer &lexer, Source &source, const bool doAST) const
        {
            assert(ground);
            XNode       *xtree = NULL;
            const Axiom *guess = NULL;
            Y_JAXIOM(std::cerr << "[" << title << "] accepting..." << std::endl);
            const bool  ok  = ground->Y_JARGON_AXIOM_ACCEPT(xtree);
            Y_JAXIOM(std::cerr << "[" << title << "] ok = " << ok << std::endl);
            Y_JAXIOM(std::cerr << "[" << title << "] by = " << (guess ? **(guess->label) : "none") << std::endl);
            
            if( ok )
            {
                //--------------------------------------------------------------
                //
                // detecting errors
                //
                //--------------------------------------------------------------
                
                //--------------------------------------------------------------
                // NULL tree..
                //--------------------------------------------------------------
                if(xtree==NULL)
                {
                    exception excp("[%s] accepted <%s> a NULL tree",**title, **(ground->label));
                    throw excp;
                }
                
                //--------------------------------------------------------------
                // check input
                //--------------------------------------------------------------
                try
                {
                    Lexeme *lexeme = lexer.get(source);
                    if(lexeme)
                    {
                        lexer.unget(lexeme);
                        throw onError("extraneous",*this,lexeme,guess);
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
                assert(NULL==xtree);
                
                //--------------------------------------------------------------
                //
                // rejected!
                //
                //--------------------------------------------------------------
                std::cerr << "[Rejected!!]" << std::endl;
                std::cerr << "with #" << lexer.lexemes.size << std::endl;
                
                exception excp("[%s] rejected",**title);
                
                
                const Lexemes &analyzed = lexer.lexemes;
                if(analyzed.size<=0)
                {
                    throw exception("[%s] invalid empty input",**title);
                }
                else
                {
                    throw onError("rejected", *this, analyzed.tail, guess);
                }
                
                
            }
            
        }
        
    }
    
}

