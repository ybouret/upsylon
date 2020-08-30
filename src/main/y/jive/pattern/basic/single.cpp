#include "y/jive/pattern/basic/single.hpp"
#include "y/information/entropy.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Single);

        Single:: ~Single() throw() {}
        Single::  Single(const uint8_t c) throw() :
        Basic(UUID),
        code(c),
        priv(0)
        { Y_PATTERN_SELF(Single);}
        
        Single::  Single(const Single &_) throw() :
        Basic(_),
        code(_.code),
        priv(0)
        { assert(UUID==uuid); Y_PATTERN_SELF(Single);}

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

        void Single:: update(Entropy &E) const throw()
        {
            ++E[code];
        }

        std::ostream & operator<<(std::ostream &os, const Single &p)
        {
            os << '\'' << cchars::visible[p.code] << '\'';
            return os;
        }

        void Single::express(ios::ostream &fp) const
        {
            Code(fp,code);
        }

    }
}
