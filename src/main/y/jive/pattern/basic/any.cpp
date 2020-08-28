
#include "y/jive/pattern/basic/any.hpp"
#include "y/information/entropy.hpp"

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

        void Any:: update(Entropy &) const throw()
        {
            return;
        }

        bool Any:: isValid(const uint8_t) const throw()
        {
            return true;
        }

        void Any:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"ANY\",shape=circle]");
        }

        void Any:: start(FirstChars &fc) const
        {
            assert(0==fc.size());
            fc.complete();
        }

        std::ostream & operator<<(std::ostream &os, const Any &)
        {
            os << "<any>";
            return os;
        }
    }
}
