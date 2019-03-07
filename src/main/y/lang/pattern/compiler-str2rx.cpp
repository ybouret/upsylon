#include "y/lang/pattern/compiler.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        string StringToRegExp(const char *text, const size_t size)
        {
            assert( !(NULL==text&&size>0) );
            string ans(size*4,as_capacity);
            for(size_t i=0;i<size;++i)
            {
                const uint8_t B = text[i];
                ans << '\\';
                ans << 'x';
                ans << hexadecimal::digit(B>>4);
                ans << hexadecimal::digit(B);
            }
            return ans;
        }

        string StringToRegExp(const string &s)
        {
            return StringToRegExp(*s,s.size());
        }

        string StringToRegExp(const char *text)
        {
            return StringToRegExp(text,length_of(text));
        }
        
        string StringToRegExp(const char C)
        {
            return StringToRegExp(&C,1);
        }
    }
}

