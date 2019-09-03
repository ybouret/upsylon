#include "y/counting/comb.hpp"
#include "y/type/bzset.hpp"


namespace upsylon {

    combination:: combination(const size_t N, const size_t K) :
    counting(compute_for(N,K)),
    n(N),
    k(K),
    nmk(n-k),
    wlen( k * sizeof(size_t) ),
    comb( acquire_(wlen) )
    {
        start();
    }

    combination:: combination(const combination &other) :
    counting(other),
    n(other.n),
    k(other.k),
    nmk(other.nmk),
    wlen( k * sizeof(size_t) ),
    comb(  acquire_(wlen) )
    {
        for(size_t i=k;i>0;--i) comb[i] = other.comb[i];
    }
    

    combination:: ~combination() throw()
    {
        release_(comb,wlen);
        bzset_(n);
        bzset_(k);
    }

    void combination:: start() throw()
    {
        (size_t&)index=1;
        for(size_t i=k;i>0;--i)
        {
            comb[i] = i;
        }
    }

    
    void combination:: next() throw()
    {
        assert(index<=count);
        if(++(size_t&)index>=count) return;

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

        size_t     res = 0;
        if( !mpn2count(res,q.num.n) )
        {
            throw exception("%s(%lu,%lu) overflow!",fn,static_cast<unsigned long>(N), static_cast<unsigned long>(K));
        }
        return res;
    }



}
