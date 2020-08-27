
#include "y/jive/pattern.hpp"
#include "y/type/block/zset.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace Jive
    {
        Pattern:: ~Pattern() throw()
        {
            _bzset(uuid);
        }

        Pattern:: Pattern(const uint32_t t) throw() :
        uuid(t)
        {
        }

        Pattern:: Pattern(const Pattern &other) throw() :
        uuid(other.uuid)
        {
        }

        size_t   Pattern:: emit_uuid(ios::ostream&fp) const
        {
            return fp.write_nbo(uuid);
        }


    }

}
