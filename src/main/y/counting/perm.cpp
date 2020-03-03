#include "y/counting/perm.hpp"
#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/type/bzset.hpp"
#include "y/type/cswap.hpp"

namespace upsylon
{

    static const char fn[] = "permutation: ";

    size_t permutation:: compute_for( const size_t N )
    {
        if(N<=0) throw exception("%sN=0",fn);
        const mpn F   = mpn::factorial(N);
        size_t    res = 0;
        if( !mpn2count(res,F) ) throw exception("%soverflow for N=%lu",fn,static_cast<unsigned long>(N));
        return res;
    }

    permutation:: permutation( const size_t N ) :
    counting( compute_for(N) ),
    accessible<size_t>(),
    n( N ),
    nm1(n-1),
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
    nm1(other.nm1),
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
        for(size_t i=n;i>0;)
        {
            perm[i] = i;
            --i;
            addr[i] = i;
        }
    }

#define SWAP(a,b) cswap(a,b)

    void permutation:: next_()  throw()
    {
        assert(index<=count);

        //----------------------------------------------------------------------
        // find the largest perm[i]
        //----------------------------------------------------------------------
        size_t i = nm1;
        while(i>0&&perm[i]>perm[i+1])
        {
            --i;
        }

        assert(i>0);
        //----------------------------------------------------------------------
        // find the largest element after perm[i] but not larger than perm[i]
        //----------------------------------------------------------------------
        {
            size_t k = n;
            while (perm[i] > perm[k])
            {
                --k;
            }
            SWAP(perm[i], perm[k]);
        }

        //----------------------------------------------------------------------
        // swap the last n - i elements
        //----------------------------------------------------------------------
        const size_t jmax=(n+i)>>1;
        for(size_t k=0,j= ++i; j <= jmax; ++j, ++k)
        {
            SWAP(perm[j], perm[n-k]);
        }
        for(size_t i=n;i>0;--i)
        {
            addr[i-1] = perm[i]-1;
        }

    }

    void permutation:: memchk(const permutation &lhs, const permutation &rhs)
    {
        assert(lhs.n==rhs.n);
        assert(lhs.count==rhs.count);
        assert(lhs.nm1==rhs.nm1);
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
