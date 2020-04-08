
#include "y/jargon/first-chars.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        FirstChars:: ~FirstChars() throw()
        {
        }
        
        FirstChars:: FirstChars() throw() : FirstCharsType()
        {
        }
        
        FirstChars:: FirstChars(size_t n) :
        FirstCharsType(n,as_capacity)
        {
        }
        
        std::ostream & operator<<(std::ostream &os, const FirstChars &fc)
        {
            os << "[";
            for(size_t i=1;i<=fc.size();++i)
            {
                os << cchars::encoded[fc[i]];
            }
            os << "]";
            return os;
        }
        
        void  FirstChars:: loadAll()
        {
            free();
            ensure(256);
            for(size_t i=0;i<256;++i)  insert( uint8_t(i) );

        }

        
        
    }
    
}
