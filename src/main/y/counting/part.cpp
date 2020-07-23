#include "y/counting/part.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/type/cswap.hpp"
#include "y/exceptions.hpp"
#include "y/counting/comb.hpp"

#include <cerrno>

namespace upsylon {

    static const char fn[] = "integer_partition: ";

    static inline size_t check_valid( const size_t value )
    {
        if(value<=0) throw libc::exception(EDOM, "%s: not for zero", fn);
        return value;
    }

    integer_partition:: integer_partition(const size_t value) :
    n( check_valid(value) ),
    m(1),
    now(0),
    tmp(0),
    wlen(2*n*sizeof(size_t))
    {
        now    = counting::acquire_(wlen);
        tmp    = now + n;
        now[1] = n;

    }

    integer_partition:: integer_partition(const integer_partition &other) :
    n(other.n),
    m(other.m),
    now(0),
    tmp(0),
    wlen(2*n*sizeof(size_t))
    {
        now    = counting::acquire_(wlen);
        tmp    = now + n;
        for(size_t i=m;i>0;--i)
        {
            now[i] = other.now[i];
            tmp[i] = other.tmp[i];
        }
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

    mpn integer_partition:: configs(const counting::with_mp_t &) const
    {
        const accessible<size_t> &self = *this;
        assert(m==self.size());
        assert(m>0);
        assert(m<=n);


#if 1
        mpn nconf = 1;

        // compute all possible choices
        for(size_t i=1, remaining=n;i<=m;++i)
        {
            const size_t groupSize = self[i];
            const mpn    numCombis = combination::compute(remaining,groupSize,counting::with_mp);
            nconf     *= numCombis;
            remaining -= groupSize;
        }

        // decrease with repetitions of groupSizes
        {
            size_t       i = m;
            while(i>0)
            {
                const size_t t = self[i];
                size_t       j = i-1;
                while(j>0&&self[j]== t)
                {
                    --j;
                }
                const size_t rep = i-j;
                i=j;
                if(rep>1)
                {
                    for(size_t j=2;j<=rep;++j)
                    {
                        const mpn J = j;
                        if( !nconf.is_divisible_by(J) ) throw exception("%sconfigs(***corrupted***)",fn);
                        nconf /= J;
                    }
                }
            }
        }
        return nconf;
#endif
    }

    size_t integer_partition:: configs(const counting::with_sz_t &)    const
    {
        size_t     ans = 0;
        const  mpn nc  = configs(counting::with_mp);
        if(!nc.as(ans)) throw exception("%sconfigurations overflow",fn);
        return ans;
    }


    integer_partition::~integer_partition() throw()
    {
        if(now>=tmp) cswap(now,tmp);
        counting::release_(now,wlen);
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
            const size_t value = now[i]; assert(value<=n); assert(value>=1);
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
            typedef core::standard<unit_t> ustd;

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
                tmp[++curr] = bk; assert(curr<=n);
            }
            if(r>0)
            {
                tmp[++curr] = r; assert(curr<=n);
            }
            _cswap(now,tmp);
            _cswap(curr,m);
            return true;
        }
    }


}
