#include "y/counting/symm-pair.hpp"
#include "y/yap/natural.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/counting/symm-indx.hpp"

namespace upsylon {

    static const char fn[] = "symm_pair: ";

    apn symm_pair::compute(const size_t n, const with_ap_t &)
    {
        if(n<=0) throw exception("%snull width",fn);
        const apn _n0  = n;
        const apn _n1  = n+1;
        apn       prod = _n0*_n1;
        return prod>>1;
    }

    size_t symm_pair:: compute( const size_t n, const with_sz_t & )
    {
        const apn ap_count  = compute(n,with_ap);
        size_t    res = 0;
        if( !ap_count.to(res) ) throw exception("%soverflow for %lu",fn,static_cast<unsigned long>(n));
        return res;
    }

    symm_pair:: ~symm_pair() throw()
    {
        _bzset(width);
    }

    symm_pair:: symm_pair(const size_t w) :
    counting( compute(w,with_sz), extent ),
    width(w),
    lower(0),
    upper(0)
    {
        boot();
    }

    symm_pair:: symm_pair(const symm_pair &other) throw() :
    collection(),
    counting( other ),
    accessible<size_t>(),
    width(other.width),
    lower(other.lower),
    upper(other.upper)
    {
        
    }
    
    std::ostream & symm_pair:: show(std::ostream &os) const
    {
        return counting::display(os, getArr(), 2);
    }
    
    size_t symm_pair:: size() const throw() { return extent; }
   
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
        symm_indx::get(aliasing::_(upper),aliasing::_(lower),index);
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
