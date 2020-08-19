
#include "y/yap/library.hpp"
#include "y/code/utils.hpp"
#include <iostream>

namespace upsylon
{

    namespace yap
    {

        static inline void output_top_byte(std::ostream &os, const uint8_t b)
        {
            const uint8_t hi = (b&0xf0)>>4;
            if(hi)
            {
                os << hexadecimal::lowercase_word[hi];
            }
            os << hexadecimal::lowercase_word[b&0x0f];

        }

        std::ostream & operator<<(std::ostream &os, const natural &n)
        {
            if(n.bytes<=0)
            {
                os << '0';
            }
            else
            {
                size_t i=n.bytes;
                output_top_byte(os,n.get(--i));
                while(i>0)
                {
                    os << hexadecimal::lowercase[ n.get(--i) ];
                }
            }
            return os;
        }

    }

}
