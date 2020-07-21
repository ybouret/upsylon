#include "y/counting/permutations.hpp"
#include "y/mpl/natural.hpp"
#include "y/type/aliasing.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include "y/associative/suffix/store.hpp"

namespace upsylon {

    permutations:: ~permutations() throw()
    {
        cleanup();
    }

    permutations:: permutations() throw():
    counting(0),
    dims(0),
    perm(0),
    shift(0),
    bytes(0)
    {}

    permutations:: permutations(const permutations &other) :
    counting(other),
    dims(other.dims),
    perm( new permutation( *other.perm) ),
    shift(0),
    bytes(other.bytes)
    {
        acquire_shift();
        for(size_t i=count;i>0;--i) aliasing::_(shift[i]) = other.shift[i];
    }

    void permutations:: acquire_shift()
    {
        assert(bytes>0);
        assert(0==shift);
        shift = static_cast<shift_t*>(memory::global::instance().acquire( aliasing::_(bytes) ));
        --shift;
    }

    const permutation & permutations:: operator*() const throw()
    {
        assert(perm.is_valid());
        return *perm;
    }


    void permutations:: cleanup() throw()
    {
        if(shift)
        {
            assert(bytes);
            memory::global::location().release( *(void **)& ++shift, aliasing::_(bytes) );
        }
        aliasing::_(dims) = 0;
    }

    void permutations:: setup(const accessible<size_t> &groups)
    {
        static const char fn[] = "permutations::setup";
        assert(groups.size()>0);

        //----------------------------------------------------------------------
        // compute metrics
        //----------------------------------------------------------------------
        const size_t  g   = groups.size();
        size_t        n   = 0; //!< will be dimensions
        mpn           den = 1; //!< denominator
        for(size_t i=g;i>0;--i)
        {
            assert(groups[i]>0);
            const size_t m = groups[i];
            n += m;
            den *= mpn::factorial(m);
        }
        const mpn    num      = mpn::factorial(n); assert(num.is_divisible_by(den));
        const mpn    mp_count = num/den;

        aliasing::_(count) = mp_count.cast_to<size_t>(fn);
        std::cerr << "count = " << count << "/" << num << std::endl;
        std::cerr << "size  = " << n  << std::endl;
        if(n>255) throw libc::exception(EDOM,"too many permutations");

        //----------------------------------------------------------------------
        // prepare base permutation and sample data
        //----------------------------------------------------------------------
        perm  = new permutation( aliasing::_(dims) = n );assert( perm->good() );
        aliasing::_(bytes) = count * sizeof(shift_t);
        acquire_shift();
        try
        {
            vector<probe_t,memory::pooled> source(n,0);
            vector<probe_t,memory::pooled> target(n,0);
            {
                probe_t value = 0;
                size_t  i     = 1;
                for(size_t j=g;j>0;--j)
                {
                    for(size_t k=groups[j];k>0;--k)
                    {
                        source[i] = target[i] = value;
                        ++i;
                    }
                    ++value;
                }
                assert(i==n+1);
                assert(size_t(value)==groups.size());
            }
            const probe_t        *key = &target[1];
            {
                suffix_store<probe_t> store;
                if(!store.insert(key,n))
                {
                    throw libc::exception(EINVAL,"%s: unexpected first sample insertion failure",fn);
                }
                permutation &p = aliasing::_(*perm);
                size_t ok   = 1;
                size_t last = 1;
                for(;p.good();p.next())
                {
                    p.make(target,source);

                    if(store.insert(key,n) )
                    {
                        const size_t curr  = p.index;
                        const size_t delta = curr-last;
                        last = curr;
                        assert(ok<count);
                        aliasing::_(shift[++ok]) = delta;
                    }

                }
                assert(count==ok);
            }
        }
        catch(...)
        {
            cleanup();
            throw;
        }
    }

    

    void permutations:: next_perm() throw()
    {
        assert(index<=count);
        permutation &self = *perm;
        size_t       ns    = shift[index]; assert(ns>0);
        while(ns-- > 0)
        {
            assert(self.good());
            self.next();
        }
    }

}
