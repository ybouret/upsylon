
#include "y/associative/suffix/stem.hpp"
#include "y/exception.hpp"

namespace upsylon {

    suffix_stem:: suffix_stem() throw() :
    nodes(0),
    created(0),
    entries(0)
    {
    }

    suffix_stem:: ~suffix_stem() throw()
    {
        assert(0==entries);
        assert(0==nodes);
        created = 0;
    }
    
    void suffix_stem:: release() throw()
    {
        free(); assert(0==entries);
        free_cache();
    }

    size_t suffix_stem:: required() const throw()
    {
        assert(nodes>=1);
        return nodes+cache_size()-1;
    }


    void suffix_stem::  throw_missing(const char *field) const
    {
        assert(field);
        throw exception("%s: missing '%s'", className(), field);
    }

    const char suffix_stem:: used[] = "used";
    const char suffix_stem:: code[] = "code";
    const char suffix_stem:: branches[] = "branches";
}

