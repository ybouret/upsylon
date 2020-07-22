#include "y/counting/comb.hpp"
#include "y/counting/ops.hpp"
#include "y/type/block/zset.hpp"


namespace upsylon {

    combination:: combination(const size_t N, const size_t K) :
    counting( compute(N,K,with_sz), K ),
    accessible<size_t>(),
    n(N),
    nmk(n-space),
    wlen( (space * sizeof(size_t)) << 1 ),
    comb( acquire_(wlen) ),
    base( comb+space )
    {
        ++base;
        boot();
    }

    combination:: combination(const combination &other) :
    collection(),
    counting(other),
    accessible<size_t>(),
    n(other.n),
    nmk(other.nmk),
    wlen( (space * sizeof(size_t) ) << 1 ),
    comb(  acquire_(wlen) ),
    base( comb+space )
    {
        ++base;
        for(size_t i=space;i>0;)
        {
            comb[i] = other.comb[i];
            --i;
            base[i] = other.base[i];
        }
    }
    

    size_t combination:: size() const throw() { return space; }

    combination:: ~combination() throw()
    {
        release_(comb,wlen);
        _bzset(n);
    }

    std::ostream & combination:: show(std::ostream &os) const
    {
        return counting::display(os,comb,space);
    }
    
    void combination:: onBoot() throw()
    {
        assert(1==index);
        core::counting::init(comb,space,base);
    }

    
    void combination:: onNext() throw()
    {
        assert(index<=count);
        
        size_t i=space;
        ++comb[i];
        while( comb[i]>nmk+i )
        {
            if(--i<=0) break;
            ++comb[i];
        }

        for(++i;i<=space;++i)
        {
            comb[i] = comb[i-1]+1;
        }

        core::counting::to_C(base,comb,space);
    }

    const size_t & combination:: operator[](const size_t j) const throw()
    {
        assert(j>=1);
        assert(j<=space);
        return comb[j];
    }

    const size_t & combination:: operator()(const size_t j) const throw()
    {
        assert(j<space);
        return base[j];
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
        assert(lhs.space==rhs.space);
        assert(lhs.count==rhs.count);
        assert(lhs.nmk==rhs.nmk);

        check_contents(
                       fn,
                       lhs, &lhs.comb[1],
                       rhs, &rhs.comb[1],
                       2*lhs.space * sizeof(size_t)
                       );
    }


    mpn  combination::  compute(const size_t N, const size_t K, const with_mp_t &)
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
        return q.num.n;
    }

    size_t combination:: compute(const size_t N, const size_t K, const with_sz_t &)
    {
        const mpn mp_count = compute(N,K,with_mp);
        size_t    res = 0;
        if( ! mp_count.as(res) )
        {
            throw exception("%s(%lu,%lu) overflow!",fn,static_cast<unsigned long>(N), static_cast<unsigned long>(K));
        }
        return res;
    }



}
