
#include "y/yap/library.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"

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


        string natural:: to_dec() const
        {
            string s;
            if(bytes<=0)
            {
                s = '0';
            }
            else
            {
                static const library &apl = library::instance();
                static const natural &ten = apl._10;
                natural self = *this;
                natural q,r;
                while(self>0)
                {
                    divide(q,r,self,ten);
                    assert(r.bytes<=1);
                    assert(r.word[0]<10);
                    s += char('0'+r.word[0]);
                    self = q;
                }
                s.reverse();
            }
            return s;
        }

        string natural:: to_hex() const
        {
            if(bytes<=0)
            {
                const string s = '0';
                return s;
            }
            else
            {
                string s(2*bytes,as_capacity,false);
                for(size_t i=bytes;i>0;)
                {
                    s += hexadecimal::lowercase[ get(--i) ];
                }
                s.skip_with('0');
                return s;
            }

        }

        std::ostream & operator<<(std::ostream &os, const natural &n)
        {

            std::ios_base::fmtflags flags = os.flags();
            if( flags &  std::ios::hex)
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
            }
            else
            {
                // assuming decimal
                const string s = n.to_dec();
                os << s;
            }
            return os;
        }

    }

}
