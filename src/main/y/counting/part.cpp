#include "y/counting/part.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/type/cswap.hpp"
#include "y/exception.hpp"
#include "y/memory/allocator.hpp"
#include "y/mpl/rational.hpp"

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

    
    size_t integer_partition:: outcomes(const size_t n)
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

    static inline
    void decrease_repeating( mpq &value, const accessible<size_t> &tab )
    {


        const size_t n = tab.size();
        size_t       i = n;
        while(i>0)
        {
            const size_t t = tab[i];
            size_t       j = i-1;
            while(j>0&&tab[j]== t)
            {
                --j;
            }
            const size_t rep = i-j;
            i=j;
            if(rep>1)
            {
                for(size_t j=2;j<=rep;++j)
                {
                    value /= j;
                }
            }
        }

    }


    mpn integer_partition:: mp_configurations() const
    {
        const accessible<size_t> &self = *this;
        assert(m>0);
        size_t remaining  = n;

        mpq q = 1;
        for(size_t i=1;i<=m;++i)
        {
            const size_t groupSize = self[i];
            for(size_t j=2;j<=remaining;++j)
            {
                q *= j;
            }
            for(size_t j=2;j<=groupSize;++j)
            {
                q /= j;
            }
            remaining -= groupSize;
            for(size_t j=2;j<=remaining;++j)
            {
                q /= j;
            }
        }
        decrease_repeating(q,self);
        if( !q.den.is_byte(1) ) throw exception("integer_partition::mp_configurations failure!!!");
        return q.num.n;
    }

    size_t integer_partition:: configurations()    const
    {
        size_t     ans = 0;
        const  mpn nc = mp_configurations();
        if(!nc.as(ans)) throw exception("integer_partion::configurations overflow");
        return ans;
    }


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
