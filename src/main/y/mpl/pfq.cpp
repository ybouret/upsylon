
#include "y/mpl/pfq.hpp"
#include <cerrno>
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace mpl
    {

        pfq:: ~pfq() throw()
        {
        }

        pfq:: pfq() :
        n(0),d(1)
        {
        }

        pfq:: pfq( const pfd &N, const pfd &D ) :
        n(N), d(D)
        {
            update();
        }

        pfq::  pfq( const natural &N, const natural &D) :
        n(N), d(D)
        {
            update();
        }


        void pfq:: update()
        {
            if( d.is_zero() )
            {
                throw libc::exception( EDOM, "mpl.pfq: division by zero");
            }

            pfd &D = (pfd &)d;

            if( n.is_zero() )
            {
                D.ld1();
            }
            else
            {
                
            }
        }

        std::ostream & operator<<( std::ostream &os, const pfq &Q)
        {
            const pfd &n = Q.n;
            const pfd &d = Q.d;
            if(n.is_zero())
            {
                assert(d.is_one());
                os << '0';
            }
            else
            {
                if(d.is_one())
                {
                    os << n;
                }
                else
                {
                    os << n << '/' << d;
                }
            }
            return os;
        }
        

    }

}

