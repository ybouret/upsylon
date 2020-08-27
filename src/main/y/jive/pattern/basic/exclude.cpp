


#include "y/jive/pattern/basic/exclude.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Exclude);

        Exclude:: ~Exclude() throw() {}
        Exclude::  Exclude(const uint8_t c)  throw() : Basic(UUID), code(c)   { Y_PATTERN_SELF(Exclude);}
        Exclude::  Exclude(const Exclude &_) throw() : Basic(_), code(_.code) { assert(UUID==uuid); Y_PATTERN_SELF(Exclude);}

        Pattern *Exclude:: clone() const
        {
            return new Exclude(*this);
        }

        Exclude     * Exclude:: Create(const uint8_t c)
        {
            return new Exclude(c);
        }


        const char *Exclude:: className() const throw()
        {
            return CLID;
        }

        size_t Exclude:: serialize(ios::ostream &fp) const
        {
            const size_t ans = id(fp);
            fp.write(code);
            return ans + 1;
        }

        bool Exclude:: isValid(const uint8_t c) const throw()
        {
            return c!=code;
        }

        void Exclude:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"" << cchars::printable[code] << "\",shape=triangle]");
        }

        void Exclude:: start(FirstChars &fc) const
        {
            assert(0==fc.size());
            fc.complete();
            fc.no(code);
        }
    }
}
