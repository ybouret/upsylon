
#include "y/yap/library.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace yap
    {

        void natural:: cast_overflow(const char *which)
        {
            if(!which) which = "???";
            throw exception("natural cast overflow for '%s'",which);
        }


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

        double natural:: to_double() const
        {
            double ans = 0;
            for(size_t i=bytes;i>0;)
            {
                ans *= 256.0;
                ans += get(--i);
            }
            return ans;
        }

        double natural:: ratio_of(const natural &num,const natural &den)
        {
            static const library &apl = library::instance();
            static const natural &ten = apl._10;
            natural q,r;
            natural::divide(q, r, num, den);
            double ans = q.to_double();

            const size_t nd  = den.bits();
            double       fac = 1;
            for(size_t i=1;i<=nd;++i)
            {
                const natural n = r*ten;
                natural::divide(q,r,n,den);
                fac *= 0.1;
                const double x = q.to_double();
                ans += fac*x;
            }
            return ans;
        }

        natural natural:: dec(const char *buffer, const size_t buflen)
        {
            static const char     fn[] = "natural::dec";
            static const library &apl  = library::instance();
            static const natural &ten  = apl._10;

            assert(buffer);
            if(buflen<=0) throw exception("%s(empty buffer)",fn);

            natural ans;
            for(size_t i=0;i<buflen;++i)
            {
                const char c = buffer[i];
                ans *= ten;
                switch(c)
                {
                    case '0': break;
                    case '1': ans += apl._1; break;
                    case '2': ans += apl._2; break;
                    case '3': ans += apl._3; break;
                    case '4': ans += apl._4; break;
                    case '5': ans += apl._5; break;
                    case '6': ans += apl._6; break;
                    case '7': ans += apl._7; break;
                    case '8': ans += apl._8; break;
                    case '9': ans += apl._9; break;
                    default:
                        throw exception("%s(invalid buffer[%u]='%s')",fn,unsigned(i),cchars::visible[ uint8_t(c) ] );

                }
            }
            return ans;
        }

        natural natural:: dec( const char *buffer )
        {
            return dec( buffer, length_of(buffer) );
        }

        natural natural:: dec( const string &buffer )
        {
            return dec( *buffer, buffer.size() );
        }

        natural natural:: hex(const char *buffer, const size_t buflen)
        {
            static const char     fn[] = "natural::hex";
            static const library &apl  = library::instance();
            static const natural &fac = apl._16;

            assert(buffer);
            if(buflen<=0) throw exception("%s(empty buffer)",fn);

            natural ans;
            for(size_t i=0;i<buflen;++i)
            {
                const char c = buffer[i];
                ans *= fac;
                switch(c)
                {
                    case '0': break;
                    case '1': ans += apl._1; break;
                    case '2': ans += apl._2; break;
                    case '3': ans += apl._3; break;
                    case '4': ans += apl._4; break;
                    case '5': ans += apl._5; break;
                    case '6': ans += apl._6; break;
                    case '7': ans += apl._7; break;
                    case '8': ans += apl._8; break;
                    case '9': ans += apl._9; break;
                    case 'a':
                    case 'A': ans += apl._10; break;
                    case 'b':
                    case 'B': ans += apl._11; break;
                    case 'c':
                    case 'C': ans += apl._12; break;
                    case 'd':
                    case 'D': ans += apl._13; break;
                    case 'e':
                    case 'E': ans += apl._14; break;
                    case 'f':
                    case 'F': ans += apl._15; break;
                    default:
                        throw exception("%s(invalid buffer[%u]='%s')",fn,unsigned(i),cchars::visible[ uint8_t(c) ] );

                }
            }
            return ans;
        }

        natural natural:: hex( const char *buffer )
        {
            return hex( buffer, length_of(buffer) );
        }

        natural natural:: hex( const string &buffer )
        {
            return hex( *buffer, buffer.size() );
        }


        natural natural:: parse(const char *buffer, const size_t buflen)
        {
            static const char     fn[] = "natural::parse";
            assert(buffer);
            switch(buflen)
            {
                case 0: throw exception("%s(empty buffer)",fn);
                case 1:
                case 2: return dec(buffer,buflen);
                default:
                    break;
            }

            if( buffer[0] == '0' && buffer[1] == 'x' )
            {
                return hex(buffer+2,buflen-2);
            }
            else
            {
                return dec(buffer,buflen);
            }
        }

        natural natural:: parse(const char *buffer)
        {
            return parse(buffer,length_of(buffer));
        }

        natural natural:: parse(const string &buffer)
        {
            return parse( *buffer, buffer.size() );
        }

    }

}
