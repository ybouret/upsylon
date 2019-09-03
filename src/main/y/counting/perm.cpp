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
    wlen( n * sizeof(size_t) ),
    perm( acquire_(wlen)     )
    {
        start();
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


    void permutation:: next()  throw()
    {
        ++(size_t&)index;
    }
}
