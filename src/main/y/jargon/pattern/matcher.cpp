
#include "y/jargon/pattern/matcher.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {

    namespace Jargon {

        Matcher:: Matcher( const string &rx ) :
        Motif( RegularExpression::Compile(rx) ),
        Token()
        {
        }

        Matcher:: Matcher( const char   *rx ) :
        Motif( RegularExpression::Compile(rx) ),
        Token()
        {
        }
        
        Matcher:: Matcher( const Matcher &other) throw():
        Motif( other ),
        Token()
        {
        }
        

        Matcher:: ~Matcher() throw()
        {
            
        }

#if 0
        const Token * Matcher:: exact_match(const string &str)
        {
            release();
            return (**this).matches_exactly(*this,str) ? this : 0;
        }
        
        const Token * Matcher:: first_match(const string &str)
        {
            release();
            return (**this).matches_partly(*this,str) ? this : 0;
        }
#endif

        
        

    }

}

