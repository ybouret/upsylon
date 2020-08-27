

#include "y/jive/pattern/basic/single.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Single);

        Single:: ~Single() throw() {}
        Single::  Single(const uint8_t c) throw() : Basic(UUID), code(c) {}
        Single::  Single(const Single &_) throw() : Basic(_), code(_.code) { assert(UUID==uuid); }

        Pattern *Single:: clone() const
        {
            return new Single(*this);
        }

        Single     * Single:: Create(const uint8_t c)
        {
            return new Single(c);
        }


        const char *Single:: className() const throw()
        {
            return CLID;
        }

        size_t Single:: serialize(ios::ostream &fp) const
        {
            return emit_uuid(fp);
        }

        bool Single:: isValid(const uint8_t c) const throw()
        {
            return c==code;
        }

    }
}
