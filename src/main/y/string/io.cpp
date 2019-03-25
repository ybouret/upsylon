#include "y/string/io.hpp"
#include "y/code/utils.hpp"

namespace  upsylon
{

    size_t string_io:: save_binary(ios::ostream &fp,
                                   const string &s)
    {
        size_t       written = 0;
        const size_t length  = s.size();
        fp.emit_upack<size_t>(length, &written);
        fp << s;
        written += length;
        return written;
    }

    string string_io:: load_binary(ios::istream &fp)
    {
        size_t n = fp.read_upack<size_t>();
        string ans(n,as_capacity);
        while(n-->0)
        {
            char C = 0;
            fp.input(&C,1);
            ans << C;
        }
        return ans;
    }

    static inline void __emit_hexa(ios::ostream &fp, const uint8_t B)
    {
        fp << '\\' << 'x' << hexadecimal::lowercase[B];
    }

    ios::ostream & string_io:: write_C(ios::ostream &fp, const string &s)
    {
        for(size_t i=0;i<s.size();++i)
        {
            const uint8_t B = s[i];
            if(B<32)
            {
                switch(B)
                {
                    case '\n': fp << '\\' << 'n'; break;
                    case '\r': fp << '\\' << 'r'; break;
                    case '\t': fp << '\\' << 't'; break;
                    default: __emit_hexa(fp,B);
                }
            }
            else if(B>=127)
            {
                __emit_hexa(fp,B);
            }
            else
            {
                switch(B)
                {
                    case '"' : fp << '\\' << '"'; break;
                    case '\'': fp << '\\' << '\''; break;
                    default:
                        fp << char(B);
                }
            }
        }
        return fp;
    }

}
