
#include "y/jive/pattern/basic/any.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Any);

        Any:: ~Any() throw() {}

        Any::  Any() throw() : Basic(UUID)
        {
            Y_PATTERN_SELF(Any);
        }

        Any::  Any(const Any &_) throw() : Basic(_)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(Any);
        }

        Pattern *Any:: clone() const
        {
            return new Any(*this);
        }

        Any     * Any:: Create()
        {
            return new Any();
        }


        const char *Any:: className() const throw()
        {
            return CLID;
        }

        size_t Any:: serialize(ios::ostream &fp) const
        {
            return id(fp);
        }

        bool Any:: isValid(const uint8_t) const throw()
        {
            return true;
        }

        void Any:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"ANY\",shape=circle]");
        }

    }
}
