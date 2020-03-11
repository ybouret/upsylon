#include "y/counting/part.hpp"
#include "y/memory/embed.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    partition::builder:: builder(const size_t value) :
    n( counting::chkdim(value) ),
    m(1),
    now(0),
    tmp(0),
    arr(0),
    wksp(0),
    wlen(0)
    {
        static memory::allocator &mmgr = counting::mem_instance();

        memory::embed emb[] =
        {
            memory::embed::as(now,n),
            memory::embed::as(tmp,n),
            memory::embed::as(arr,n),
        };
        wksp = memory::embed::create(emb,
                                     sizeof(emb)/sizeof(emb[0]),
                                     mmgr,
                                     wlen);
        --now;
        --tmp;
        now[1] = n;
        update();
    }


    void partition::builder::init() throw()
    {
        now[1] = n;
        aliasing::_(m) = 1;
        update();
    }

    void partition::builder::update() throw()
    {
        for(size_t i=m;i>0;)
        {
            const size_t value = now[i--] - 1;
            arr[i] = value;
        }
    }

    size_t partition::builder:: size() const throw()
    {
        return m;
    }


    size_t partition::builder:: count_for(const size_t n)
    {
        partition::builder pb(n);
        pb.init();
        size_t ans = 1;
        while( pb.next() ) ++ans;
        return ans;
    }


    const size_t & partition::builder:: operator[](size_t i) const
    {
        assert(i>0);
        assert(i<=m);
        return now[i];
    }

    partition::builder::~builder() throw()
    {
        static memory::allocator &mmgr = counting::mem_location();
        mmgr.release(wksp, wlen);
        now = 0;
        tmp = 0;
        arr = 0;
        aliasing::_(n) = 0;
        aliasing::_(m) = 0;
    }

    bool partition::builder:: next() throw()
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
            for(size_t j=k+1;j<=k+q;++j)
            {
                tmp[++curr] = bk; assert(j==curr);
            }
            if(r>0)
            {
                tmp[++curr] = r;
            }
            _cswap(now,tmp);
            _cswap(curr,m);
            update();
            return true;
        }
    }


}
