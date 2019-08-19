
#include "y/mpl/mpn.hpp"
#include "y/exceptions.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"
#include "y/ios/ostream.hpp"

#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        static inline bool is0(const char C) throw()
        {
            return '0' == C;
        }


        std::ostream & natural:: display( std::ostream &os ) const
        {
            static const MPN &_ = MPN::instance();

            std::ios_base::fmtflags flags = os.flags();
            if( flags &  std::ios::hex)
            {
                //______________________________________________________________
                //
                // hexadecimal
                //______________________________________________________________
                if(bytes<=0)
                {
                    os << "0x0";
                }
                else
                {
                    string s(2+(bytes<<1),as_capacity);
                    for(size_t i=0;i<bytes;++i)
                    {
                        const char *h = hexadecimal::lowercase[byte[i]];
                        s << h[1] << h[0];
                    }
                    (s.trim(is0) << "x0").reverse();
                    os << s;
                }
            }
            else //assuming decimal
            {
                //______________________________________________________________
                //
                // decimal
                //______________________________________________________________
                if(bytes<=0)
                {
                    os << '0';
                }
                else
                {
                    const natural ten = _._10;
                    natural q,r,num=*this;
                    string s;
                    while(true)
                    {
                        // num=q*10+r
                        split(q,r,num,ten);

                        switch(r.bytes)
                        {
                            case 0:  s << '0'; break;
                            case 1: {
                                const unsigned b = r.byte[0];
                                if(b<=0||b>9) throw exception("mpn.display(remainder failure: r=%u)", b);
                                s << char('0'+b);
                            } break;
                            default:
                                throw exception("mpn.display(remainder #bytes=%u failure)", unsigned(bytes));
                        }
                        if(q.is_zero()) break;
                        num.xch(q);
                    }
                    s.reverse();
                    os << s;
                }

            }
           
            return os;
        }

        double natural:: to_real() const
        {
            double ans = 0;
            for(size_t i=bytes;i>0;--i)
            {
                ans *= 256;
                ans += item[i];
            }
            return ans;
        }

        double natural:: ratio_of(const natural &num,const natural &den)
        {
            static const MPN &_ = MPN::instance();
            natural q,r;
            natural::split(q, r, num, den);
            double ans = q.to_real();

            const size_t nd  = den.bits();
            double       fac = 1;
            for(size_t i=1;i<=nd;++i)
            {
                const natural n = r*_._10;
                natural::split(q,r,n,den);
                fac *= 0.1;
                const double x = q.to_real();
                ans += fac*x;
            }
            return ans;
        }

        natural natural:: hex( const string &s )
        {
            if(s.size()<=0) throw libc::exception(EDOM,"natural.hex(empty.string)");
            natural ans;
            for(size_t i=0;i<s.size();++i)
            {
                const char c = s[i];
                const int  d = hexadecimal::to_decimal(c);
                if(d<0) throw libc::exception(EDOM,"natural.hex(invalid char '%c')", c);

                (ans <<= 4) |= d;
            }

            return ans;
        }

        natural natural:: hex( const char *s )
        {
            const string _(s);
            return hex(_);
        }


        natural natural:: dec( const string &s )
        {
            if(s.size()<=0) throw libc::exception(EDOM,"natural.dec(empty.string)");
            natural ans;
            const mpn &ten = MPN::instance()._10;
            for(size_t i=0;i<s.size();++i)
            {
                const char c = s[i];
                if(c<'0'||c>'9') throw libc::exception(EDOM,"natural.dec(invalid char '%c')", c);
                const mpn  d = word_t(c-'0');
                ans *= ten;
                ans += d;
            }
            return ans;
        }

        natural natural:: dec( const char *s )
        {
            const string _(s);
            return dec(_);
        }

        natural natural:: parse( const string &s )
        {
            if( (s.size()>=2) && ('0' == s[0]) && ('x' == s[1] || 'X' == s[1] ) )
            {
                const string _( &s[2], s.size()-2 );
                return hex(_);
            }
            else
            {
                return dec(s);
            }
        }

        natural natural:: parse( const char *s )
        {
            const string _(s);
            return parse(_);
        }

        size_t natural:: serialize( ios::ostream &fp ) const
        {
            return fp.emit_block(*this);
        }


        const char natural:: CLASS_NAME[] = "mpn";
        
        const char * natural:: className() const throw()
        {
            return CLASS_NAME;
        }

        
#if 0
        natural natural:: read( ios::istream &fp, size_t *nr)
        {
            const size_t num_bytes = fp.read_upack<size_t>(nr);
            mpn          ans(num_bytes,as_capacity);
            fp.input(ans.byte,(ans.bytes=num_bytes));
            ans.upgrade();
            if(ans.bytes!=num_bytes) throw exception("mpn.read(corrupted bytes)");
            if(nr) *nr += num_bytes;
            return ans;
        }
#endif
        
        natural natural:: read( ios::istream &fp )
        {
            const size_t num_bytes = fp.read_upack<size_t>();
            mpn          ans(num_bytes,as_capacity);
            fp.input(ans.byte,(ans.bytes=num_bytes));
            ans.upgrade();
            if(ans.bytes!=num_bytes) throw exception("mpn.read(corrupted bytes)");
            return ans;
        }
        
    }

}

