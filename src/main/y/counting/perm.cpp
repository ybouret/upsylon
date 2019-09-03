#include "y/counting/perm.hpp"
#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/type/bzset.hpp"

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
    n( N ),
    nm1(n-1),
    wlen( n * sizeof(size_t) ),
    perm( acquire_(wlen)     )
    {
        start();
    }

    permutation:: permutation(const permutation &other) :
    counting(other),
    n(other.n),
    nm1(other.nm1),
    wlen(n * sizeof(size_t)),
    perm( acquire_(wlen)     )
    {
        for(size_t i=n;i>0;--i) perm[i] = other.perm[i];
    }

    permutation:: ~permutation() throw()
    {
        release_(perm,wlen);
        bzset_(n);
    }

    void permutation:: start() throw()
    {
        (size_t&)index=1;
        for(size_t i=n;i>0;--i) perm[i] = i;
    }

#define SWAP(a,b) cswap(a,b)

    void permutation:: next()  throw()
    {
        assert(index<=count);

        if(++(size_t&)index>count) return;

        //std::cerr << "@index=" << index << std::endl;

        // find the largest perm[i]
        size_t i = nm1;
        while(i>0&&perm[i]>perm[i+1])
        {
            --i;
        }

        assert(i>0);
        /* Find the largest element after perm[i] but not larger than perm[i] */
        size_t k = n;
        while (perm[i] > perm[k])
        {
            --k;
        }
        SWAP(perm[i], perm[k]);

        /* Swap the last n - i elements. */
        //std::cerr << "i=" << i <<  ", n=" << n << std::endl;
        k = 0;
        for(size_t j = i+1; j <= (n + i) / 2; ++j, ++k)
        {
            //std::cerr << "swap(" << j << "," << n-k << ")" << std::endl;
            SWAP(perm[j], perm[n-k]);
        }

    }

    void permutation:: memchk(const permutation &lhs, const permutation &rhs)
    {
        assert(lhs.n==rhs.n);
        assert(lhs.count==rhs.count);
        assert(lhs.nm1==rhs.nm1);
        const int value = memcmp( &lhs.perm[1], &rhs.perm[1], lhs.n * sizeof(size_t) );
        if(lhs.index==rhs.index)
        {
            if(value!=0) throw exception("%ssame index, different components!",fn);
        }
        else
        {
            if(value==0) throw exception("%sdifferent indices, same components!",fn);
        }
    }

}
