#include "y/counting/permutations.hpp"
#include "y/mpl/natural.hpp"
#include "y/type/aliasing.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/associative/suffix/store.hpp"

namespace upsylon {

    permutations_:: ~permutations_() throw()
    {
        cleanup();
    }

    permutations_:: permutations_() throw():
    counting(0,0),
    perm(0),
    shift(0),
    bytes(0)
    {}

    permutations_:: permutations_(const permutations_ &other) :
    counting(other),
    perm( new permutation( *other.perm) ),
    shift(0),
    bytes(other.bytes)
    {
        acquire_shift();
        for(size_t i=count;i>0;--i) aliasing::_(shift[i]) = other.shift[i];
    }

    void permutations_:: acquire_shift()
    {
        assert(bytes>0);
        assert(0==shift);
        shift = static_cast<shift_t*>(memory::global::instance().acquire( aliasing::_(bytes) ));
        --shift;
    }

    const permutation & permutations_:: operator*() const throw()
    {
        assert(perm.is_valid());
        return *perm;
    }


    void permutations_:: cleanup() throw()
    {
        if(shift)
        {
            assert(bytes);
            memory::global::location().release( *(void **)& ++shift, aliasing::_(bytes) );
        }
    }

    mpl::natural permutations_:: compute(size_t &sum, const accessible<size_t> &groups, const with_mp_t &)
    {
        assert(0==sum);
        mpn den = 1;
        for(size_t i=groups.size();i>0;--i)
        {
            const size_t m = groups[i];
            const mpn    f = mpn::factorial(m);
            sum += m;
            den *= f;
        }
        const mpn num = mpn::factorial(sum); assert(num.is_divisible_by(den));
        return num/den;
    }

    size_t permutations_::compute(size_t &sum,const accessible<size_t> &groups, const with_sz_t &)
    {
        const mpn ans = compute(sum,groups,with_mp);
        return ans.cast_to<size_t>("permutations::compute");
    }


    void permutations_:: setup(const accessible<size_t> &groups)
    {
        static const char fn[] = "permutations::setup";
        assert(groups.size()>0);

        //----------------------------------------------------------------------
        //
        // compute metrics
        //
        //----------------------------------------------------------------------
        aliasing::_(count) = compute(aliasing::_(space),groups,with_sz);
        if(space>255) throw libc::exception(EDOM,"too many permutations");

        //----------------------------------------------------------------------
        //
        // prepare base permutation and sample data
        //
        //----------------------------------------------------------------------
        perm  = new permutation( aliasing::_(space) );assert( perm->good() );
        aliasing::_(bytes) = count * sizeof(shift_t);
        acquire_shift();
        try
        {
            vector<probe_t,memory::pooled> source(space,0);
            vector<probe_t,memory::pooled> target(space,0);
            {
                probe_t value = 0;
                size_t  i     = 1;
                for(size_t j=groups.size();j>0;--j)
                {
                    for(size_t k=groups[j];k>0;--k)
                    {
                        source[i] = target[i] = value;
                        ++i;
                    }
                    ++value;
                }
                assert(i==space+1);
                assert(size_t(value)==groups.size());
            }
            const probe_t        *key = &target[1];
            {
                //--------------------------------------------------------------
                // initialize store
                //--------------------------------------------------------------
                suffix_store<probe_t> store;
                if(!store.insert(key,space)) throw libc::exception(EINVAL,"%s: unexpected first sample insertion failure",fn);

                //--------------------------------------------------------------
                // loop over permutations
                //--------------------------------------------------------------
                permutation &p    = aliasing::_(*perm);
                size_t       ok   = 1;
                size_t       last = 1;
                for(;p.good();p.next())
                {
                    p.make(target,source);

                    if(store.insert(key,space))
                    {
                        //------------------------------------------------------
                        // a new key
                        //------------------------------------------------------
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

    

    void permutations_:: next_perm() throw()
    {
        assert(index<=count);
        permutation &self = *perm;
        size_t       ns   = shift[index]; assert(ns>0);
        while(ns-- > 0)
        {
            assert(self.good());
            self.next();
        }
    }

}
