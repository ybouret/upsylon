#include "y/counting/permuter.hpp"
#include "y/counting/perm-ops.hpp"
#include "y/type/block/zset.hpp"
#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace core {

        static const char fn[] = "permuter";

        permuter:: ~permuter() throw()
        {
            _bzset(dims);
            _bzset(perm);
        }

        permuter:: permuter(const size_t n) :
        upsylon::counting(0),
        dims(n),
        weak(false),
        classes(0),
        current(),
        perm(0)
        {
            if(dims<=0) throw libc::exception(ERANGE,"%s has no dimension",fn);
        }

        permuter:: permuter(const permuter &other) throw() :
        upsylon::counting(other),
        dims(other.dims),
        weak(other.weak),
        classes(other.classes),
        current(),
        perm(0)
        {

        }

        void permuter:: init_once() throw()
        {
            new ( aliasing::anonymous( (void*) &current ) ) indices(perm+1,dims);
        }

        mpn permuter:: count_with(const repeats &reps, const upsylon::counting::with_mp_t &) const
        {
            aliasing::_(weak) = false;
            mpn res = mpn::factorial(dims);
            for(const repeat *rep = reps.head; rep; rep=rep->next )
            {
                if(rep->data>1)
                {
                    aliasing::_(weak) = true;
                    const mpn den = mpn::factorial(rep->data);
                    if( ! res.is_divisible_by(den) )
                    {
                        throw exception("%s count corruption",fn);
                    }
                    res /= den;
                }

            }
            return res;
        }

        size_t permuter:: count_with(const repeats &reps, const upsylon::counting::with_sz_t &) const
        {
            const mpn mp = count_with(reps,upsylon::counting::with_mp);
            size_t    res=0;
            if( !mp.as(res) )
            {
                throw libc::exception( ERANGE, "%s overflow for dims=%u",fn,unsigned(dims));
            }
            return res;
        }

        void permuter:: init_perm() throw()
        {
            core::counting::init(perm,dims);
        }

        void permuter:: next_perm() throw()
        {
            core::permutation::next(perm,dims);
        }

        void permuter:: invalid_first_key() const
        {
            throw exception("%s unexpected invalid first key!",fn);
        }


        size_t permuter:: save_state(ios::ostream &fp, const suffix_stem &stem) const
        {
            // save only good configuration
            if(!good()) throw exception("permuter::save_state(not good!)");

            // save index
            size_t ans = fp.write_upack(index);

            // save permutation state
            for(size_t i=1;i<=dims;++i)
            {
                ans += fp.write_upack(perm[i]);
            }

            // save tree
            ans += stem.serialize(fp);
            
            return ans;
        }

        size_t permuter:: load_perm(ios::istream &fp)  
        {
            static const char fn[] = "permuter::load_perm";
            boot();
            try {
                size_t ans = 0;
                {
                    size_t shift = 0;
                    if(!fp.query_upack(aliasing::_(index), shift)) throw exception("%s: missing index",fn);
                    if(index<=0||index>count)                      throw exception("%s: invalid index",fn);
                    ans += shift;
                }

                for(size_t i=1;i<=dims;++i)
                {
                    size_t shift = 0;
                    if(!fp.query_upack(perm[i],shift)) throw exception("%s: missing perm[%u]",fn,unsigned(i));
                    ans += shift;
                }
                
                return ans;
            }
            catch(...)
            {
                boot();
                throw;
            }
        }

    }

}
