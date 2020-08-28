
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
            static const char quote = '\'';
            os << '{';
            const size_t n=fc.size();
            if(n>0)
            {
                size_t i =1;
                while(i<=n)
                {
                    os << ' ';
                    size_t   a=fc[i];
                    size_t   d=1;
                    size_t   j=i+1;
                    for(;j<=n;++j,++d)
                    {
                        if(a+d!=fc[j]) break;
                    }
                    i=j;
                    if(1==d)
                    {
                        os << quote << cchars::visible[a] << quote;
                    }
                    else
                    {
                        os << '[' <<quote << cchars::visible[a] << quote << '-' << quote << cchars::visible[ fc[--j] ] << quote << ']';
                    }
                }
                os << ' ';
            }
            os << '}';
            return os;
        }

        

    }

}
