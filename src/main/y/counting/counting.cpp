
#include "y/counting/counting.hpp"
#include "y/type/bzset.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    counting:: counting(const size_t n) throw() : index(0), count(n) {}

    counting:: ~counting() throw()
    {
        bzset_(index);
        bzset_(count);
    }

    counting:: counting( const counting &other) throw() :
    index( other.index ),
    count( other.count )
    {
    }
    
    bool counting:: valid() const throw()
    {
        assert(index>0);
        return (index<=count);
    }

    memory::allocator & counting:: mem_instance()
    {
        return memory::pooled::instance();
    }

    memory::allocator & counting:: mem_location() throw()
    {
        return memory::pooled::location();
    }


    size_t * counting:: acquire_( size_t &bytes )
    {
        static memory::allocator &mgr = mem_instance();
        assert(bytes>0);
        assert(0==(bytes%sizeof(size_t)));
        return static_cast<size_t *>( mgr.acquire(bytes) )-1;
    }

    void counting:: release_( size_t * &comb, size_t &bytes ) throw()
    {
        static memory::allocator &mgr = mem_location();
        ++comb;
        mgr.release(*(void **)&comb,bytes);
        assert(0==comb);
        assert(0==bytes);
    }


}

#include "y/mpl/natural.hpp"

namespace upsylon
{

    bool counting:: mpn2count(size_t &sz, const mpl::natural &n )
    {
        static const size_t sz_bytes = sizeof(size_t);
        static const size_t sz_bits  = sz_bytes * 8;

        sz=0;
        if(n.bits()>sz_bits) return false;
        sz = n.byte_at(0);
        for(size_t i=1;i<sz_bytes;++i)
        {
            const size_t B = n.byte_at(i);
            sz |= (B << (i*8));
        }
        return true;
    }

    std::ostream & counting:: display( std::ostream &os, const size_t *arr, const size_t num )
    {
        os << '{';
        os << arr[1];
        for(size_t i=2;i<=num;++i) os << ',' << arr[i];
        os << '}';
        return os;
    }

    void counting:: start() throw()
    {
        (size_t &)index = 1;
        start_();
    }

    void counting:: next() throw()
    {
        if( ++(size_t &)index <= count ) next_();
    }

}

#include "y/exception.hpp"

namespace upsylon
{

    size_t counting:: chkdim(const size_t dim)
    {
        if(dim<=0) throw exception("counting: zero dimension!!!");
        return dim;
    }

    void counting:: check_contents(const char     *id,
                                   const counting &lhs, const void *l,
                                   const counting &rhs, const void *r,
                                   const size_t    length)
    {
        assert(id); assert(l); assert(r); assert(length>0);
        assert(lhs.count==rhs.count);

        const int status = memcmp(l,r,length);
        if(lhs.index==rhs.index)
        {
            if(0!=status) throw exception("%s same index but different content!!",id);
        }
        else
        {
            if(0==status) throw exception("%s different indices (#lhs=%u,#rhs=%u) but same content!!",id, unsigned(lhs.index), unsigned(rhs.index) );
        }
    }



}
