
#include "y/associative/suffix/stem.hpp"

namespace upsylon {

    suffix_stem:: suffix_stem() throw() :
    nodes(0),
    created(0)
    {
    }

    suffix_stem:: ~suffix_stem() throw()
    {
    }
    
    void suffix_stem:: release() throw()
    {
        free();
        free_cache();
    }

    size_t suffix_stem:: required() const throw()
    {
        assert(nodes>=1);
        return nodes+cache_size()-1;
    }

}

