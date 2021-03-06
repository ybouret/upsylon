
#include "y/counting/counting.hpp"
#include "y/type/block/zset.hpp"
#include "y/memory/allocator.hpp"
#include <cstring>

namespace upsylon
{
    counting:: counting(const size_t n, const size_t s) throw() : index(0), count(n), space(s)
    {
    }

    counting:: ~counting() throw()
    {
        _bzset(index);
        _bzset(count);
        _bzset(space);
    }

    counting:: counting( const counting &other) throw() :
    index( other.index ),
    count( other.count ),
    space( other.space )
    {
    }
    
    bool counting:: good() const throw()
    {
        assert(index>0);
        return (index<=count);
    }

   


    size_t * counting:: acquire_( size_t &bytes )
    {
        static memory::allocator &mgr = instance();
        assert(bytes>0);
        assert(0==(bytes%sizeof(size_t)));
        return static_cast<size_t *>( mgr.acquire(bytes) )-1;
    }

    void counting:: release_( size_t * &comb, size_t &bytes ) throw()
    {
        static memory::allocator &mgr = location();
        ++comb;
        mgr.release(*(void **)&comb,bytes);
        assert(0==comb);
        assert(0==bytes);
    }

    const counting::with_sz_t counting::with_sz = {};
    const counting::with_ap_t counting::with_ap = {};

}

#include <iostream>
#include "y/type/aliasing.hpp"
#include "y/parops.hpp"

namespace upsylon
{
    
    
    std::ostream & counting:: display( std::ostream &os, const size_t *arr, const size_t num )
    {
        os << '{';
        os << arr[1];
        for(size_t i=2;i<=num;++i) os << ',' << arr[i];
        os << '}';
        return os;
    }

    void counting:: boot()
    {
        aliasing::_(index) = 1;
        onBoot();
    }

    void counting:: next()
    {
        try {
            if( aliasing::incr(index) <= count ) onNext();
        }
        catch(...)
        {
            aliasing::decr(index);
            throw;
        }
    }

    size_t counting:: boot(const size_t global_size,
                           const size_t global_rank)
    {
        boot();
        size_t length = count;
        size_t offset = 1;
        parops::split_any(length,offset,global_size,global_rank);
        while(offset-- > 1) next();
        return length;
    }

    
    void counting:: unwind()
    {
        while( good() ) next();
    }
}

#include "y/exception.hpp"

namespace upsylon
{

    
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

#include "y/utest/timings.hpp"

namespace upsylon {

    double counting:: loops_per_second(const double duration)
    {
        double speed = 0;
        unwind();
        Y_TIMINGS(speed,duration,boot();while(good()) next());
        return speed;
    }

}
