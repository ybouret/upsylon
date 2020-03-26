#include "y/jargon/pattern/matching.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        Matching:: Matching(const Motif &m) throw() :
        Motif(m)
        {
        }
        
        Matching:: Matching(Pattern *p) throw() :
        Motif(p)
        {
        }
        
        Matching:: Matching(const string &rx, const Dictionary *dict):
        Motif( RegularExpression::Compile(rx,dict) )
        {
        }
        
        Matching:: Matching(const char *rx, const Dictionary *dict):
        Motif( RegularExpression::Compile(rx,dict) )
        {
        }
        
        Matching:: Matching(const Matching &other) throw() :
        Motif(other)
        {
        }
        
        Matching:: ~Matching() throw()
        {
        }
        
        bool Matching:: exactly(Token &token, const string &s)
        {
            Source source( token.cache, Module::OpenData(token.cache,s) );
            token.erase();
            const Pattern &p = **this;
            if( p.match(token,source) )
            {
                return source.isEmpty();
            }
            else
            {
                return false;
            }
        }
        
        bool Matching:: partly(Token &token, const string &s)
        {
            Source source( token.cache, Module::OpenData(token.cache,s) );
            token.erase();
            const Pattern &p = **this;
            while( source.isAlive() )
            {
                if( p.match(token,source) )
                {
                    return true;
                }
                source.skip(1);
            }
            
            return false;
            
        }
        
        

        
        
        
    }
}
