
#include "y/type/rtti.hpp"
#include "y/type/aliasing.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon
{
    typedef hashing::fnv rtti_hasher;

    size_t rtti:: hash(const char *id) throw()
    {
        assert(id);
        return rtti_hasher::of(id);
    }

    size_t rtti:: hash(const std::type_info &tid) throw()
    {
        return hash( tid.name() );
    }

    size_t rtti:: hash(const string &id) throw()
    {
        return hash(*id);
    }

    rtti :: rtti(const std::type_info &tid) :
    name( tid.name() ),
    code( hash(name) ),
    user()
    {
    }

    rtti:: ~rtti() throw()
    {
        aliasing::_(code) = 0;
    }

}


