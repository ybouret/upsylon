

#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        

        
        static inline void writeLexeme(exception     &excp,
                                       const Lexeme  &L,
                                       const Grammar &G)
        {
            
            const Tag        &label = L.label;
            uint32_t          uuid  = 0;
            const AxiomStatus kind  = G.statusOf(label,uuid);
            
            const char *lid = **label;
            const char *tid = "";
            switch(uuid)
            {
                case Terminal::UUID: tid = TextFor::Terminal; break;
                case Operator::UUID: tid = TextFor::Operator; break;
                default:
                    break;
            }
            
            switch(kind)
            {
                case ExternalAxiom: // lexical error handling...
                    L.writeTo(excp,false);
                    break;
                    
                case NoLexemeAxiom:
                    excp.cat("unexpected <%s> (%s), check grammar!!!",lid,fourcc_(uuid));
                    break;
                    
                    
                case DefiniteAxiom:
                    excp.cat("%s ", tid); L.writeTo(excp,true);
                    break;
                    
                case FlexibleAxiom:
                    excp.cat("%s ", tid); L.writeTo(excp,false);
                    break;
                    
            }
            
            
            
        }
        
        static inline exception syntaxError(const char    *reason,
                                            const Grammar  &G,
                                            const Lexeme   &L)
        {
            assert(reason);
            exception       excp("%s:%d:%d: [%s] %s ",
                                 **(L.tag),
                                 L.line,
                                 L.column,
                                 **G.title, reason);
            writeLexeme(excp,L,G);
            if(L.prev)
            {
                excp.cat(" after ");
                writeLexeme(excp,*(L.prev),G);
            }
            return excp;
        }
        
        XNode * Grammar:: accept(Lexer &lexer, Source &source) const
        {
            assert(ground);
            XNode       *xtree = NULL;
            Y_JAXIOM(std::cerr << "[" << title << "] accepting..." << std::endl);
            const bool  ok  = ground->Y_JARGON_AXIOM_ACCEPT(xtree);
            Y_JAXIOM(std::cerr << "[" << title << "] ok = " << ok << std::endl);
            
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
                        XNode::Restore(xtree,lexer);
                        xtree = NULL;
                        throw syntaxError("Extraneous", *this, *lexeme);
                    }
                    
                }
                catch(...)
                {
                    if(xtree) delete xtree;
                    throw;
                }
                
                //--------------------------------------------------------------
                //
                // ok, everything seems good
                //
                //--------------------------------------------------------------
                if(buildAST)
                {
                    Y_JAXIOM(std::cerr << "[" << title << "] returning AST" << std::endl);
                    return AST(xtree);
                }
                else
                {
                    Y_JAXIOM(std::cerr << "[" << title << "] returning Raw Tree" << std::endl);
                    return xtree;
                }
                
            }
            else
            {
                assert(NULL==xtree);
                
                //--------------------------------------------------------------
                //
                // rejected!
                //
                //--------------------------------------------------------------
                exception      excp("[%s] rejected",**title);
                const Lexemes &analyzed = lexer.lexemes;
                if(analyzed.size<=0)
                {
                    throw exception("[%s] invalid empty input",**title);
                }
                else
                {
                    assert(analyzed.tail);
                    throw syntaxError("Rejected", *this, *analyzed.tail);
                }
                
                
            }
            
        }
        
    }
    
}

