
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

        FirstChars:: FirstChars(const FirstChars &other) :
        collection(), fcOrdered(other)
        {
        }


        void FirstChars:: complete()
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

        void FirstChars:: opposite(const FirstChars &fc)
        {
            if(this==&fc || size() >= 256)
            {
                free();
            }
            else
            {
                const size_t n   = fc.size();
                const size_t num = 256-n;
                free(); ensure(num);
                for(unsigned i=0;i<256;++i)
                {
                    const uint8_t value = uint8_t(i);
                    if( !fc.search(value) )
                    {
                        insert(value);
                    }
                }
                assert( size() == num );
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
