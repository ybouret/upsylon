
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
            os << '[';
            const size_t n = fc.size();
            for(size_t i=1;i<=n;)
            {
                const size_t Ci = fc[i];
                size_t       ns = 0;
                for(size_t j=i+1,delta=1;j<=n;++j,++delta)
                {
                    const size_t Cj = fc[j];
                    if(Ci+delta==Cj)
                    {
                        ++ns;
                    }
                    else
                    {
                        break;
                    }
                }
                switch(ns)
                {
                    case 0:
                        os << cchars::encoded[Ci]; ++i;
                        break;
                        
                    case 1:
                        os << cchars::encoded[Ci]; ++i;
                        os << cchars::encoded[fc[i]]; ++i;
                        break;
                        
                    default:
                        os << cchars::encoded[Ci] << '-';
                        i+=ns;
                        os << cchars::encoded[fc[i]]; ++i;
                        break;
                }
            }
            os << ']';
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
