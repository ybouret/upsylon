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
        _bzset(dimension);
    }

    symm_pair:: symm_pair(const size_t dim) :
    counting( compute(dim,with_sz) ),
    dimension(dim),
    lower(0),
    upper(0)
    {
        boot();
    }

    symm_pair:: symm_pair(const symm_pair &other) throw() :
    collection(),
    counting( other ),
    accessible<size_t>(),
    dimension(other.dimension),
    lower(other.lower),
    upper(other.upper)
    {
        
    }
    
    std::ostream & symm_pair:: show(std::ostream &os) const
    {
        return counting::display(os, getArr(), 2);
    }
    
    size_t symm_pair:: size() const throw() { return 2; }
   
    const size_t * symm_pair:: getArr() const throw()
    {
        const size_t *p = &lower;
        return (--p);
    }
    
    const size_t & symm_pair:: operator[](const size_t i) const throw()
    {
        assert(i>=1);
        assert(i<=2);
        return *( getArr()+i );
    }
    
    void symm_pair:: update() throw()
    {
        symm_indx(aliasing::_(upper),aliasing::_(lower),index);
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
