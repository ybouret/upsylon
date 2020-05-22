
#include "y/jargon/motifs.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {

    namespace Jargon {

        Motifs:: Motifs( const string &rx ) : Motif( RegularExpression::Compile(rx) )
        {
        }

        Motifs:: Motifs( const char   *rx ) : Motif( RegularExpression::Compile(rx) )
        {
        }

        Motifs:: ~Motifs() throw()
        {
            
        }

        Motifs:: Motifs( const Motifs &other) throw() : Motif(other)
        {
        }
        

    }

}

