
#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace mpl
    {
        static inline bool is0(const char C) throw()
        {
            return '0' == C;
        }


        void natural:: display( std::ostream &os ) const
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
            else if( flags & std::ios::dec )
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
                                s << ('0'+b);
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
            else
            {
                throw exception("natural.display: unsupported format");
            }
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

    }

}

