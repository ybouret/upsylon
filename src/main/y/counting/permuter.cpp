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
        perm(0)
        {
            if(dims<=0) throw libc::exception(ERANGE,"%s has no dimension",fn);
        }

        permuter:: permuter(const permuter &other) throw() :
        upsylon::counting(other),
        dims(other.dims),
        weak(other.weak),
        classes(other.classes),
        perm(0)
        {

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
        
    }

}
