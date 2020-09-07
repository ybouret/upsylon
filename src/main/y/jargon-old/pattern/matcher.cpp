
#include "y/jargon/pattern/matcher.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {

    namespace Jargon {

        Matcher:: Matcher(const string &rx, const Dictionary *dict) :
        Motif( RegularExpression::Compile(rx,dict) ),
        Token()
        {
        }

        Matcher:: Matcher(const char   *rx, const Dictionary *dict) :
        Motif( RegularExpression::Compile(rx,dict) ),
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

        void Matcher:: ignoreCase()
        {
            Motif  target = (**this).ignoreCase();
            Motif &source = *this;
            source = target;
        }
        
        
        

    }

}

