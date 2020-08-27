

#include "y/jive/pattern/basic/single.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Single);

        Single:: ~Single() throw() {}
        Single::  Single(const uint8_t c) throw() : Basic(UUID), code(c)   { Y_PATTERN_SELF(Single);}
        Single::  Single(const Single &_) throw() : Basic(_), code(_.code) { assert(UUID==uuid); Y_PATTERN_SELF(Single);}

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
            const size_t ans = id(fp);
            fp.write(code);
            return ans + 1;
        }

        bool Single:: isValid(const uint8_t c) const throw()
        {
            return c==code;
        }

        void Single:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"" << cchars::printable[code] << "\",shape=square]");
        }

        void Single:: start(FirstChars &fc) const
        {
            assert( fc.size() == 0);
            fc.insert(code);
        }
    }
}
