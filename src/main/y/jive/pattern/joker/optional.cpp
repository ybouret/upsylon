

#include "y/jive/pattern/joker/optional.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Optional);

        Optional:: ~Optional() throw() {}

        Optional::  Optional(const Motif &m) throw() : Joker(UUID,m)
        {
            Y_PATTERN_SELF(Optional);
        }

        Optional::  Optional(const Optional &_) throw() :
        Joker(_)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(Optional);
        }

        Pattern *Optional:: clone() const
        {
            return new Optional(*this);
        }

        Optional     * Optional:: Create(Pattern *p)
        {
            const Motif m(p);
            return new Optional(m);
        }


        const char *Optional:: className() const throw()
        {
            return CLID;
        }

        size_t Optional:: serialize(ios::ostream &fp) const
        {
            const size_t ans = id(fp);
            return ans + motif->serialize(fp);
        }


        void Optional:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"?\",shape=diamond]");
            vizLink(fp);
        }

        bool Optional:: feeble() const throw()
        {
            return true;
        }

        bool Optional:: accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
            if( motif->accept(token,source) )
            {
                return true;
            }
            else
                return true;
        }


#if 0
        std::ostream & operator<<(std::ostream &os, const Any &)
        {
            os << "<any>";
            return os;
        }
#endif
        
    }
}
