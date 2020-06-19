#include "y/counting/symm-pair.hpp"
#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/counting/symm-indx.hpp"

namespace upsylon {

    static const char fn[] = "symm_pair: ";

    mpn symm_pair::compute(const size_t n, const with_mp_t &)
    {
        if(n<=0) throw exception("%snull dimension",fn);
        const mpn _n0  = n;
        const mpn _n1  = n+1;
        mpn       prod = _n0*_n1;
        return prod>>1;
    }

    size_t symm_pair:: compute( const size_t n, const with_sz_t & )
    {
        const mpn mp_count  = compute(n,with_mp);
        size_t    res = 0;
        if( !mp_count.as(res) ) throw exception("%soverflow for n=%lu",fn,static_cast<unsigned long>(n));
        return res;
    }

    symm_pair:: ~symm_pair() throw()
    {
        _bzset(n);
    }

    symm_pair:: symm_pair(const size_t dim) :
    counting( compute(dim,with_sz) ),
    n(dim),
    i(0),
    j(0)
    {
        boot();
    }

    void symm_pair:: update() throw()
    {
        symm_indx(aliasing::_(i), aliasing::_(j),index);
    }

    void symm_pair:: onBoot() throw()
    {
        assert(1==index);
        update();
    }


    void symm_pair:: onNext() throw()
    {
        assert(index<=count);
        update();
    }


}
