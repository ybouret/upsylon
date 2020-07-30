#include "y/jargon/pattern.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        bool  Pattern:: exactlyMatches(Token &token, Module *m) const
        {
            Source source(m);
            token.release();
            if(this->match(token,source))
            {
                return source.isEmpty();
            }
            else
            {
                return false;
            }
        }

        bool Pattern:: somehowMatches(Token &token, Module *m) const
        {
            Source source(m);
            token.release();
            while( source.isAlive() )
            {
                if( this->match(token,source) )
                {
                    return true;
                }
                source.skip(1);
            }
            return false;
        }

        size_t Pattern:: findAllMatches(sequence<Token> &tokens, Module *m) const
        {
            Source source(m);
            Token  token;
            size_t count = 9;
            while( source.isAlive() )
            {
                if( this->match(token,source) )
                {
                    tokens.push_back(token);
                    source.unget(token);
                }
                source.skip(1);
            }
            return count;
        }

        
        
    }
}
