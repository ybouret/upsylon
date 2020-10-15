#include "y/counting/perm.hpp"
#include "y/yap/natural.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"
#include "y/counting/perm-ops.hpp"
#include "y/counting/ops.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{

    static const char fn[] = "permutation: ";

    apn permutation:: compute( const size_t N, const with_ap_t & )
    {
        if(N<=0) throw exception("%sN=0",fn);
        return apn::factorial(N);
    }

    size_t permutation:: compute( const size_t N, const with_sz_t & )
    {
        const apn count = compute(N,with_ap);
        size_t    res   = 0;
        if( !count.to(res) ) throw exception("%soverflow for N=%lu",fn,static_cast<unsigned long>(N));
        return res;
    }

    permutation:: permutation( const size_t N ) :
    counting( compute(N,with_sz), N ),
    accessible<size_t>(),
    wlen( 2*space* sizeof(size_t) ),
    perm( acquire_(wlen)  ),
    addr( &perm[1]+space  )
    {
        boot();
    }

    permutation:: permutation(const permutation &other) :
    collection(),
    counting(other),
    accessible<size_t>(),
    wlen( 2*space* sizeof(size_t)),
    perm( acquire_(wlen) ),
    addr( &perm[1]+space )
    {
        for(size_t i=space;i>0;)
        {
            perm[i] = other.perm[i];
            --i;
            addr[i] = other.addr[i];
        }
        assert( are_equal(*this,other) );
    }

    permutation:: ~permutation() throw()
    {
        release_(perm,wlen);
    }

    size_t permutation:: size() const throw() { return space; }

    std::ostream & permutation:: show(std::ostream &os) const
    {
        return counting::display(os,perm,space);
    }
    
    void permutation:: onBoot() throw()
    {
        assert(1==index);
        core::counting::init(perm,space,addr);
    }
    
    void permutation:: onNext()  throw()
    {
        assert(index<=count);
        core::permutation::_nxt(perm,space);
        core::counting::to_C(addr,perm,space);
    }

    void permutation:: memchk(const permutation &lhs, const permutation &rhs)
    {
        assert(lhs.space==rhs.space);
        assert(lhs.count==rhs.count);
        check_contents(fn, lhs, &lhs.perm[1], rhs, &rhs.perm[1], lhs.space * sizeof(size_t));
    }
    
#define Y_CHECK_PERM_CXX_INDX()     \
assert(indx>0);                     \
assert(indx<=space);                \
assert(perm[indx]>0);               \
assert(perm[indx]<=space)
    
    const size_t & permutation:: operator[](const size_t indx) const throw()
    {
        Y_CHECK_PERM_CXX_INDX();
        return perm[indx];
    }
    
#define Y_CHECK_PERM_C_INDX() \
assert(indx<space);           \
assert(addr[indx]<space)
    
    const size_t & permutation:: operator()(const size_t indx) const throw()
    {
        Y_CHECK_PERM_C_INDX();
        return addr[indx];
    }

    bool permutation:: are_equal(const permutation &lhs, const permutation &rhs) throw()
    {
        if( lhs.size() != rhs.size() ) return false;
        if( lhs.count  != rhs.count  ) return false;
        if( lhs.index  != rhs.index  ) return false;
        for(size_t i=lhs.size();i>0;--i)
        {
            if( lhs.perm[i] != rhs.perm[i] ) return false;
            assert( lhs.addr[i-1] == rhs.addr[i-1] );
        }
        return true;
    }

    void permutation:: reload( const permutation &p ) throw()
    {
        assert( this != &p );
        assert(size()==p.size());
        assert(count==p.count);
        aliasing::_(index) = p.index;
        for(size_t i=0,j=1;i<space;++i,++j)
        {
            addr[i] = p.addr[i];
            perm[j] = p.perm[j];
        }
        assert( are_equal(*this,p) );
    }



}
