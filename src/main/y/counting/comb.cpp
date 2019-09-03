#include "y/counting/comb.hpp"
#include "y/type/bzset.hpp"
#include "y/memory/pooled.hpp"

#include <iostream>

namespace upsylon {

    namespace
    {
        static inline size_t *comb_acquire( size_t &bytes )
        {
            static memory::allocator &mgr = memory::pooled::instance();
            assert(bytes>0);
            assert(0==(bytes%sizeof(size_t)));
            return static_cast<size_t *>( mgr.acquire(bytes) )-1;
        }

        static inline void comb_release( size_t * &comb, size_t &bytes ) throw()
        {
            static memory::allocator &mgr = memory::pooled::location();
            ++comb;
            mgr.release(*(void **)&comb,bytes);
            assert(0==comb);
            assert(0==bytes);
        }
    }


    combination:: combination(const size_t N, const size_t K) :
    n(N),
    k(K),
    index(0),
    count( compute_for(n,k)  ),
    nmk(n-k),
    wlen( k * sizeof(size_t) ),
    comb( comb_acquire(wlen) )
    {
        start();
    }

    combination:: combination(const combination &other) :
    n(other.n),
    k(other.k),
    index(other.index),
    count(other.count),
    nmk(other.nmk),
    wlen( k * sizeof(size_t) ),
    comb( comb_acquire(wlen) )
    {
        for(size_t i=k;i>0;--i) comb[i] = other.comb[i];
    }
    

    combination:: ~combination() throw()
    {
        comb_release(comb,wlen);
        bzset_(n);
        bzset_(k);
        bzset_(index);
        bzset_(count);
    }

    void combination:: start() throw()
    {
        (size_t&)index=1;
        for(size_t i=k;i>0;--i)
        {
            comb[i] = i;
        }
    }

    bool combination:: active() const throw()
    {
        return (index<=count);
    }

    void combination:: next() throw()
    {
        assert(index<=count);
        size_t i=k;
        ++comb[i];
        while( comb[i]>nmk+i )
        {
            if(--i<=0) break;
            ++comb[i];
        }

        for(++i;i<=k;++i)
        {
            comb[i] = comb[i-1]+1;
        }

        ++(size_t &)index;
    }

    const size_t & combination:: operator[](const size_t j) const throw()
    {
        assert(j>=1);
        assert(j<=k);
        return comb[j];
    }




}

#include "y/mpl/rational.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    static const char fn[]="combination: ";

    void combination:: memchk(const combination &lhs, const combination &rhs)
    {
        assert(lhs.n==rhs.n);
        assert(lhs.k==rhs.k);
        assert(lhs.count==rhs.count);
        assert(lhs.nmk==rhs.nmk);
        const int value = memcmp( &lhs.comb[1], &rhs.comb[1], lhs.k * sizeof(size_t) );
        if(lhs.index==rhs.index)
        {
            if(value!=0) throw exception("%ssame index, different components!",fn);
        }
        else
        {
            if(value==0) throw exception("%sdifferent indices, same components!",fn);
        }
    }

    std::ostream &  operator<<( std::ostream &os, const combination &c)
    {
        os << '{' << c.comb[1];
        for(size_t j=2;j<=c.k;++j) os << ',' << c.comb[j];
        os << '}';
        return os;
    }
    size_t combination:: compute_for(const size_t N, const size_t K)
    {
        if(N<=0) throw exception("%s(N=0)",fn);
        if(K<=0) throw exception("%s(K=0)",fn);
        if(K>N)  throw exception("%s(K=%lu>N=%lu)",fn,static_cast<unsigned long>(K), static_cast<unsigned long>(N));

        mpq          q(1);
        const size_t dlo = 1;
        const size_t dhi = K;
        const size_t nlo = N+1-K;
        const size_t nhi = N;

        size_t i=nlo;
        size_t j=dlo;
        while(i<=nhi&&j<=dhi)
        {
            q *= i++;
            q /= j++;
        }

        while(j<=dhi)
        {
            q /= j++;
        }

        while(i<=nhi)
        {
            q *= i++;
        }

        if( ! q.den.is_byte(1) ) throw exception("%sFAILURE!",fn);

        const mpn &ans = q.num.n;
        if(ans.bits()>sizeof(size_t)*8) throw exception("%s(%lu,%lu) overflow!",fn,static_cast<unsigned long>(N), static_cast<unsigned long>(K));

        size_t res = ans.byte_at(0);
        for(size_t i=1;i<sizeof(size_t);++i)
        {
            const size_t B = ans.byte_at(i);
            res |= (B << (i*8));
        }
        assert(res==ans);
        return res;
    }



}
