#include "y/jargon/pattern.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        bool  Pattern:: exactly_matches(Token &token, Module *m) const
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

        bool Pattern:: somehow_matches(Token &token, Module *m) const
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

        size_t Pattern:: get_all_matches( sequence<Token> &tokens, Module *m) const
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

#if 0
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
#endif
        

        
        
        
    }
}
