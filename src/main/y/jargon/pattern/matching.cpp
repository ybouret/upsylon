#include "y/jargon/pattern/matching.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        Matching:: Matching( const Cache &c, const Motif &m) throw() :
        Token(c),
        Motif(m)
        {
        }
        
        Matching:: Matching( const Cache &c, Pattern *p) throw() :
        Token(c),
        Motif(p)
        {
        }
        
        Matching:: Matching( const Cache &c,const string &rx, const Dictionary *dict):
        Token(c),
        Motif( RegularExpression::Compile(rx,dict) )
        {
        }
        
        Matching:: Matching( const Cache &c,const char *rx, const Dictionary *dict):
        Token(c),
        Motif( RegularExpression::Compile(rx,dict) )
        {
        }
        
        Matching:: ~Matching() throw()
        {
        }
        
        
    }
}
