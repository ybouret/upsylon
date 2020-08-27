
#include "y/jive/pattern/first-chars.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    namespace Jive
    {
        FirstChars:: ~FirstChars() throw()
        {
        }

        FirstChars:: FirstChars() : fcOrdered(4,as_capacity)
        {
        }

        

        void FirstChars:: fulfill()
        {
            if(capacity()<256)
            {
                reserve(256-capacity());
            }
            
            for(unsigned i=0;i<256;++i)
            {
                insert( uint8_t(i) );
            }

        }

        std::ostream & operator<<(std::ostream &os, const FirstChars &fc )
        {
            os << '{';
            const size_t n=fc.size();
            if(n>0)
            {
                os << cchars::visible[ fc[1] ];
                for(size_t i=2;i<=n;++i)
                {
                    os << ' ' << cchars::visible[ fc[i] ];
                }
            }
            os << '}';
            return os;
        }

    }

}
