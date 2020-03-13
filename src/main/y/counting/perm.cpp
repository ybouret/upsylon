#include "y/counting/perm.hpp"
#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"
#include "y/counting/permops.hpp"

namespace upsylon
{

    static const char fn[] = "permutation: ";

    mpn permutation:: compute( const size_t N, const with_mp_t & )
    {
        if(N<=0) throw exception("%sN=0",fn);
        return mpn::factorial(N);
    }

    size_t permutation:: compute( const size_t N, const with_sz_t & )
    {
        const mpn mp_count  = compute(N,with_mp);
        size_t    res = 0;
        if( !mp_count.as(res) ) throw exception("%soverflow for N=%lu",fn,static_cast<unsigned long>(N));
        return res;
    }

    permutation:: permutation( const size_t N ) :
    counting( compute(N,with_sz) ),
    accessible<size_t>(),
    n( N ),
    wlen( 2*n * sizeof(size_t) ),
    perm( acquire_(wlen)       ),
    addr(perm+n+1)
    {
        start();
    }

    permutation:: permutation(const permutation &other) :
    collection(),
    counting(other),
    accessible<size_t>(),
    n(other.n),
    wlen( 2*n * sizeof(size_t)),
    perm( acquire_(wlen)     ),
    addr(perm+n+1)
    {
        for(size_t i=n;i>0;)
        {
            perm[i] = other.perm[i];
            --i;
            addr[i] = other.addr[i];
        }
    }

    permutation:: ~permutation() throw()
    {
        release_(perm,wlen);
        _bzset(n);
    }

    size_t permutation:: size() const throw() { return n; }


    void permutation:: start_() throw()
    {
        assert(1==index);
        permops::init(perm,n,addr);
    }

   

    void permutation:: next_()  throw()
    {
        assert(index<=count);
        permops::next(perm,n);
        permops::to_C(addr,perm,n);
    }

    void permutation:: memchk(const permutation &lhs, const permutation &rhs)
    {
        assert(lhs.n==rhs.n);
        assert(lhs.count==rhs.count);
        //assert(lhs.nm1==rhs.nm1);
        check_contents(fn, lhs, &lhs.perm[1], rhs, &rhs.perm[1], lhs.n * sizeof(size_t));
    }
    
#define Y_CHECK_PERM_CXX_INDX() \
assert(indx>0);             \
assert(indx<=n);            \
assert(perm[indx]>0);       \
assert(perm[indx]<=n)
    
    const size_t & permutation:: operator[](const size_t indx) const throw()
    {
        Y_CHECK_PERM_CXX_INDX();
        return perm[indx];
    }
    
#define Y_CHECK_PERM_C_INDX() \
assert(indx<n);               \
assert(addr[indx]<n)
    
    const size_t & permutation:: operator()(const size_t indx) const throw()
    {
        Y_CHECK_PERM_C_INDX();
        return addr[indx];
    }

}
