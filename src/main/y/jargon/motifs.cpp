
#include "y/jargon/motifs.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {

    namespace Jargon {

        Motifs:: Motifs( const string &rx ) :
        Motif( RegularExpression::Compile(rx) ),
        Token()
        {
        }

        Motifs:: Motifs( const char   *rx ) :
        Motif( RegularExpression::Compile(rx) ),
        Token()
        {
        }
        
        Motifs:: Motifs( const Motifs &other) throw():
        Motif( other ),
        Token()
        {
        }
        

        Motifs:: ~Motifs() throw()
        {
            
        }
        
        const Token * Motifs:: exact_match(const string &str)
        {
            release();
            return (**this).matches_exactly(*this,str) ? this : 0;
        }
        
        const Token * Motifs:: first_match(const string &str)
        {
            release();
            return (**this).matches_partly(*this,str) ? this : 0;
        }
        

        
        

    }

}

