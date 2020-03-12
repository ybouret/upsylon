#include "y/counting/part.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/type/cswap.hpp"
#include "y/exception.hpp"
#include "y/memory/allocator.hpp"

namespace upsylon {

    integer_partition:: integer_partition(const size_t value) :
    n( counting::chkdim(value) ),
    m(1),
    now(0),
    tmp(0),
    wlen(2*n*sizeof(size_t))
    {
        static memory::allocator &mmgr = counting::mem_instance();
        now = static_cast<size_t *>(mmgr.acquire(wlen));

        tmp    = (--now) + n;
        now[1] = n;
    }


    void integer_partition:: initialize() throw()
    {
        now[1]         = n;
        aliasing::_(m) = 1;
    }



    size_t integer_partition:: size() const throw()
    {
        return m;
    }

    size_t integer_partition:: outcomes() throw()
    {
        initialize();
        size_t ans = 1;
        while( build_next() )
            ++ans;
        initialize();
        return ans;
    }

    
    size_t integer_partition:: count_for(const size_t n)
    {
        integer_partition pb(n);
        return pb.outcomes();
    }


    const size_t & integer_partition:: operator[](size_t i) const
    {
        assert(i>0);
        assert(i<=m);
        assert(i<=n);
        return now[i];
    }

    
    
#if 0
    size_t partition::builder:: permutations() const
    {
        const accessible<size_t> &self = *this;
        mpn p = mpn::factorial(m);
        for(size_t i=m;i>0;)
        {
            const size_t value = self[i];
            size_t       j     = i-1;
            while(j>0&&self[j]==value)
                --j;
            const size_t rep = i-j;
            //std::cerr << '\t' << '[' << value << ']' << '$' << rep << std::endl;
            i=j;
            if(rep>1)
            {
                const mpn d = mpn::factorial(rep);
                p/=d;
            }
        }
        size_t np = 0;
        if( !counting::mpn2count(np,p) )
        {
            throw exception("#permutations overflow in partition::builder");
        }
        return np;
    }
#endif


    integer_partition::~integer_partition() throw()
    {
        static memory::allocator &mmgr = counting::mem_location();
        { void *addr = ++now;     mmgr.release(addr, wlen); }
        now = 0;
        tmp = 0;
        aliasing::_(n) = 0;
        aliasing::_(m) = 0;
    }

    bool integer_partition:: build_next() throw()
    {
        size_t curr= 0;
        size_t k   = 0;
        size_t N   = 0;
        for(size_t i=1;i<=m;++i)
        {
            const unsigned value = now[i]; assert(value<=n); assert(value>=1);
            if(value<=1)
            {
                ++N;
            }
            else
            {
                k=i;
            }
        }
        if(N>=n)
        {
            assert(m==n);
            return false;
        }
        else
        {
            typedef standard<unit_t> ustd;

            for(size_t j=1;j<k;++j)
            {
                tmp[j] = now[j];
            }
            assert(now[k]>1);
            const size_t   bk = tmp[curr=k] = now[k]-1;
            ustd::div_type d  = ustd::div_call(N+1,bk);
            const size_t   q  = d.quot;
            const size_t   r  = d.rem;
            for(size_t j=1;j<=q;++j)
            {
                tmp[++curr] = bk; //assert(j==curr);
            }
            if(r>0)
            {
                tmp[++curr] = r;
            }
            _cswap(now,tmp);
            _cswap(curr,m);
            return true;
        }
    }


}
