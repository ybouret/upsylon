
#include "y/mpl/pfq.hpp"
#include "y/sequence/list.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

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

        pfq:: pfq(const pfq &other) :
        object(),
        counted_object(),
        n(other.n),d(other.d)
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

        pfq:: pfq( const word_t N, const word_t D) :
        n(N), d(D)
        {
            update();
        }

        pfq:: pfq( const pfd &N ) :
        n(N), d(1)
        {
        }

        pfq:: pfq( const natural &N ) :
        n(N), d(1)
        {
        }

        pfq:: pfq( const word_t N ) :
        n(N), d(1)
        {
        }
        
        void pfq:: update()
        {
            // no division by zero
            if( d.is_zero() )
            {
                throw libc::exception( EDOM, "mpl.pfq: division by zero");
            }

            // first case
            if( n.is_zero() )
            {
                ((pfd &)d).ld1();
                return;
            }

            // second case: nothing to do is n==1 || d==1
            if( n.is_one() || d.is_one() )
            {
                return;
            }

            // third case : equality
            if( n == d )
            {
                ((pfd &)d).ld1();
                ((pfd &)n).ld1();
                return;
            }

            _pfd::table &N = (_pfd::table &)(n.table);
            _pfd::table &D = (_pfd::table &)(d.table);

            list<mpn> removeD, removeN;
            for( _pfd::table::iterator j=D.begin(); j!=D.end(); ++j)
            {
                _pfd &          DD   = **j;
                const mpn      &p    = DD.p;
                _pfd::pointer * ppNN = N.search(p);

                if(ppNN)
                {
                    // common factor
                    _pfd & NN = **ppNN;
                    const  int cmp = mpn::compare(NN.q,DD.q);
                    if(cmp<0)
                    {
                        assert(NN.q<DD.q);
                        DD.q -= NN.q;
                        removeN.push_back(p);
                    }
                    else
                    {
                        if(cmp>0)
                        {
                            assert(NN.q>DD.q);
                            NN.q -= DD.q;
                            removeD.push_back(p);
                        }
                        else
                        {
                            assert(NN.q==DD.q); // to remove
                            removeD.push_back(p);
                            removeN.push_back(p);
                        }
                    }
                }
            }

            for( list<mpn>::iterator i=removeD.begin(); i != removeD.end();++i)
            {
                D.no(*i);
            }
            if(D.size()<=0) ( (pfd &)d ).ld1();

            for( list<mpn>::iterator i=removeN.begin(); i != removeN.end();++i)
            {
                N.no(*i);
            }
            if(N.size()<=0) ( (pfd &)n ).ld1();
            
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

        void pfq:: mul_by( const pfd &other )
        {
            ((pfd &)n).mul_by(other);
            update();
        }

        void pfq:: div_by( const pfd &other )
        {
            ((pfd &)d).mul_by(other);
            update();
        }

        void pfq:: mul_by( const pfq &other )
        {
            ((pfd &)d).mul_by(other.d);
            ((pfd &)n).mul_by(other.n);
            update();
        }

        void pfq:: div_by( const pfq &other )
        {
            ((pfd &)d).mul_by(other.n);
            ((pfd &)n).mul_by(other.d);
            update();
        }

        pfd pfq:: A(const word_t N, const word_t K)
        {
            if(K>N) throw libc::exception(EDOM,"A(N=%lu<K=%lu)", (unsigned long)N, (unsigned long)K);
            const pfd num = pfd::factorial(N);
            const pfd den = pfd::factorial(N-K);
            const pfq Q(num,den);
            if(!Q.d.is_one()) throw exception("unexpected failure in A(N=%lu,K=%lu)", (unsigned long)N, (unsigned long)K);
            return Q.n;
        }

        pfd pfq:: C(const word_t N, const word_t K)
        {
            if(K>N) throw libc::exception(EDOM,"C(N=%lu<K=%lu)", (unsigned long)N, (unsigned long)K);
            const pfd num = pfd::factorial(N);
            const pfd den = pfd::factorial(N-K);
            pfq Q(num,den);
            {
                const pfd KF = pfd::factorial(K);
                Q.div_by(KF);
            }
            if(!Q.d.is_one()) throw exception("unexpected failure in C(N=%lu,K=%lu)", (unsigned long)N, (unsigned long)K);
            return Q.n;
        }

    }

}

