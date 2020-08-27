
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
        uuid(t), self(0)
        {
        }

        Pattern:: Pattern(const Pattern &other) throw() :
        uuid(other.uuid), self(0)
        {
        }

        size_t   Pattern:: id(ios::ostream&fp) const
        {
            return fp.write_nbo(uuid);
        }

        
    }

}
