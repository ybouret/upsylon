#include "y/mpl/rational.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"

#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        void rational:: check()
        {
            if(den.is_zero())
            {
                throw libc::exception(EDOM,"mpl.rational: zero denominator");
            }
            __simplify();
        }

        void rational:: __simplify()
        {
            assert(den.is_positive());
            natural &d = (natural &) den;
            integer &z = (integer &) num;
            if(z.s==__zero)
            {
                // numerator is null => denominator to 1
                assert(true==z.n.is_zero());
                if(!d.is_byte(1))
                {
                    d.set_byte(1);
                }
            }
            else
            {
                // numerator is not zero, obviously not for denominator
                assert( z.s   != __zero        );
                assert( false == z.n.is_zero() );

                if(!d.is_byte(1))
                {
                    natural      &n = (natural &)(z.n);
                    natural      a=n;
                    natural      b=d;
                    if(b>a)
                    {
                        a.xch(b);
                    }
                    // b<=a
                    while( b.is_positive() )
                    {
                        natural m = natural::__mod(a,b);
                        a.xch(b);
                        b.xch(m);
                    }
                    // a is the gcd
                    n/=a;
                    d/=a;
                }
            }
        }

        rational rational:: __add( const rational &lhs, const rational &rhs )
        {
            const natural dd = lhs.den * rhs.den;
            const integer ad = lhs.num * rhs.den;
            const integer cb = rhs.num * lhs.den;
            const integer nn = ad+cb;
            return rational(nn,dd);
        }

        rational rational::__sub( const rational &lhs, const rational &rhs )
        {
            const natural dd = lhs.den * rhs.den;
            const integer ad = lhs.num * rhs.den;
            const integer cb = rhs.num * lhs.den;
            const integer nn = ad-cb;
            return rational(nn,dd);
        }

        rational rational::__mul( const rational &lhs, const rational &rhs )
        {
            const integer new_num = lhs.num * rhs.num;
            const natural new_den = lhs.den * rhs.den;
            return rational(new_num,new_den);
        }

        rational rational:: square_of(const rational &q)
        {
            const integer new_num = integer::square_of(q.num);
            const natural new_den = natural::square_of(q.den);
            return rational(new_num,new_den);
        }



        rational rational::__div( const rational &lhs, const rational &rhs )
        {
            const integer i_num = lhs.num * rhs.den;
            const integer i_den = lhs.den * rhs.num;
            const integer o_num( sign_ops::product(i_num.s,i_den.s), i_num.n);
            return rational(o_num,i_den.n);
        }


        rational::rational() : number_type(), num(),den(1) {}
        rational::~rational() throw() {}
        rational::rational( const integer_t i ) :  object(), number_type(), num(i), den(1) {}
        rational::rational( const integer & z ) :  object(), number_type(), num(z), den(1) {}
        rational::rational( const natural & n ) :  object(), number_type(), num(n), den(1) {}
        rational::rational( const integer_t n, const word_t  d) :  object(), number_type(), num(n), den(d) { check(); }
        rational::rational( const integer &z, const natural &u) :  object(), number_type(), num(z), den(u) { check(); }

        std::ostream & operator<<( std::ostream &os, const rational &q )
        {
            if(q.den.is_byte(1))
            {
                os << q.num;
            }
            else
            {
                os << '(' << q.num << '/' << q.den << ')';
            }
            return os;
        }

        double rational:: to_real() const throw()
        {
            switch(num.s)
            {
                case __negative: return -natural::ratio_of(num.n,den);
                case __positive: return  natural::ratio_of(num.n,den);
                case __zero:     return 0;
            }
#if defined(__ICC)
            fatal_error("intel corrupted code@mpl.rational.to_real");
            return 0;
#endif
#if defined(__GNUC__)
            fatal_error("gnu corrupted code@mpl.rational.to_real");
            return 0;
#endif
#if defined(_MSC_VER)
            fatal_error("MSC corrupted code@mpl.rational.to_real");
            return 0;
#endif
        }

        rational:: rational( const rational &q ) :  object(), number_type(), num(q.num), den(q.den) {}

        void rational::xch( rational &q ) throw()
        {
            ( (integer &)num ).xch( (integer &)(q.num) );
            ( (natural &)den ).xch( (natural &)(q.den) );
        }

        rational & rational:: operator=( const rational &q )
        {
            rational tmp(q);
            xch(tmp);
            return *this;
        }

        rational & rational:: operator=( const integer_t i )
        {
            rational tmp(i); xch(tmp); return *this;
        }

        rational & rational:: operator=( const integer & z )
        {
            rational tmp(z); xch(tmp); return *this;
        }

        rational & rational:: operator=( const natural & n )
        {
            rational tmp(n); xch(tmp); return *this;
        }


        int rational::compare( const rational &lhs, const rational &rhs)
        {
            const integer L = lhs.num * rhs.den;
            const integer R = rhs.num * lhs.den;
            return integer::compare(L,R);
        }

        rational rational::operator+() const
        {
            return *this;
        }

        rational rational::__inc() const
        {
            const integer new_num = num + den;
            return rational(new_num,den);
        }

        rational & rational::operator++()  { rational tmp = __inc(); xch(tmp); return *this; }

        rational rational::operator++(int) { rational tmp = __inc(); xch(tmp); return tmp; }


        rational rational:: operator-() const
        {
            const  integer new_num( sign_ops::neg(num.s), num.n );
            return rational(new_num,den);
        }

        rational rational:: __dec() const
        {
            const integer new_num = num - den;
            return rational(new_num,den);
        }

        rational & rational::operator--()  { rational tmp = __dec(); xch(tmp); return *this; }

        rational rational::operator--(int) { rational tmp = __dec(); xch(tmp); return tmp; }

        const char rational:: CLASS_NAME[] = "mpq";

        const char * rational:: className() const throw()
        {
            return CLASS_NAME;
        }

    }

}

#include "y/ios/gist.hpp"

namespace upsylon {

    namespace mpl
    {
        size_t rational:: serialize( ios::ostream &fp ) const
        {
            const size_t bytes_for_num = num.serialize(fp);
            const size_t bytes_for_den = den.serialize(fp);
            return bytes_for_num+bytes_for_den;
        }

        rational rational:: read( ios::istream &fp, size_t *shift)
        {
            size_t nn = 0, nd=0;
            const integer _num = integer::read(fp,&nn);
            const natural _den = natural::read(fp,&nd);
            ios::gist::assign(shift,nd+nn);
            return rational(_num,_den);
        }
    }
}
