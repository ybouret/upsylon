#include "y/mpl/sprp.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {

        sprp:: ~sprp() throw()
        {

        }

        sprp:: sprp( const sprp &other ) :
        n(other.n),
        m(other.m),
        s(other.s),
        d(other.d)
        {

        }

        sprp:: sprp( const natural &value ) :
        n(value),
        m(n),
        s(0),
        d(0)
        {
            static const char fn[] = "mpl.sprp.odd_dcmp";
            if( n.is_byte(0) || n.is_byte(1) || n.is_byte(2) )
            {
                throw libc::exception( EDOM, "%s(invalid n=%u)", fn, unsigned( n[0] ) );
            }
            else if(n.is_even())
            {
                throw libc::exception( EDOM, "%s(even natural value)", fn);
            }
            else
            {
                assert(n>=3);
                natural &M = (natural &)m;
                natural &S = (natural &)s;
                natural &D = (natural &)d;
                D = --M; assert(M==D); assert(D.is_even());

                while( D.is_even() )
                {
                    ++S;
                    D.shr();
                }


#ifndef NDEBUG
                natural q = 1;
                for(natural i=s;i>0;--i)
                {
                    q *= 2;
                }
                q *= d;
                ++q;
                assert(q==n);
#endif
            }

        }

        std::ostream & operator<<( std::ostream &os, const sprp &dcmp )
        {
            assert(dcmp.s>0);
            os << '(';
            os << '2';

            if(dcmp.s>1)
            {
                os << '^' << dcmp.s;
            }

            if(dcmp.d>1)
            {
                os << '*' << dcmp.d;
            }

            os << '+' << '1';
            os << ')';
            return os;
        }

        bool sprp:: is_for( const mpn &a ) const
        {
            {
                const natural q = natural::mod_exp(a,d,n);
                if(q.is_byte(1)) return true;
            }

            return false;
        }


    }

}
