#include "y/counting/permutations.hpp"
#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"

namespace upsylon {

    permutations:: ~permutations() throw()
    {
    }

    permutations:: permutations() throw()
    {
    }


    void permutations:: setup(const size_t *group, const size_t groups)
    {
        static const char fn[] = "permutations::setup";

        assert(group);
        assert(groups>0);

        // compute metrics
        size_t n   = 0;
        mpn    den = 1;
        for(size_t i=0;i<groups;++i)
        {
            assert(group[i]>0);
            const size_t m = group[i];
            n += m;
            den *= mpn::factorial(m);
        }
        const mpn    num      = mpn::factorial(n); assert(num.is_divisible_by(den));
        const mpn    mp_count = num/den;

        size_t count = mp_count.cast_to<size_t>(fn);
        std::cerr << "count=" << count << "/" << num << std::endl;

    }

}
