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
        l(other.l)
        {

        }

        sprp:: sprp( const word_t value ) :
        n(value),
        m(n),
        s(0),
        l()
        {
            setup();
        }


        void sprp:: setup()
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
                list_type &L = (list_type&)l;
                natural   &M = (natural &)m;
                natural   &S = (natural &)s;
                --M;
                L.push_back( new node_type(M) );

                natural &D = (natural &)(L.head->q);
                assert(M==D); assert(D.is_even());

                while( D.is_even() )
                {
                    ++S;
                    D.shr();
                }
                if(S<=0) throw exception("%s(corrupted algorithm)",fn);

                for(mpn i=S;i>1;--i)
                {
                    mpn tmp = L.tail->q;
                    L.push_back( new node_type(tmp.shl()) );
                }

                assert(S==l.size);


#ifndef NDEBUG
                natural q = 1;
                for(natural i=s;i>0;--i)
                {
                    q *= 2;
                }
                q *= D;
                ++q;
                assert(q==n);
#endif

            }

        }

        sprp:: sprp( const natural &value ) :
        n(value),
        m(n),
        s(0),
        l()
        {
            setup();
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

            const natural &d = dcmp.l.head->q;
            if(d>1)
            {
                os << '*' << d;
            }

            os << '+' << '1';
            os << ')';
            return os;
        }

        bool sprp:: operator()( const mpn &a ) const
        {
            if(a<2) throw exception("mpl.sprp<2");
            
            const node_type *node = l.head;
            {
                const natural &d = node->q;
                const natural  q = natural::mod_exp(a,d,n);
                if(q.is_byte(1)) return true;
                if(q==m)         return true;
            }

            for(node=node->next;node;node=node->next)
            {
                const natural q = natural::mod_exp(a,node->q,n);
                if(q==m) return true;
            }


            return false;
        }


    }

}
