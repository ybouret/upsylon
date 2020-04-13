#include "y/jargon/pattern.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        
        
        bool Pattern:: matches_exactly(Token &token, const string &s) const
        {
            Source source( Module::OpenData(s) );
            token.release();
            const Pattern &p = *this;
            if( p.match(token,source) )
            {
                return source.isEmpty();
            }
            else
            {
                return false;
            }
        }
        
        bool Pattern:: matches_partly(Token &token, const string &s) const
        {
            Source source( Module::OpenData(s) );
            token.release();
            const Pattern &p = *this;
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
