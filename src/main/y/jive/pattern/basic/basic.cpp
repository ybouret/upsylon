
#include "y/jive/pattern/basic/basic.hpp"

namespace upsylon {

    namespace Jive
    {
        Basic:: ~Basic() throw()
        {
        }

        Basic:: Basic(const uint32_t t) throw() : Pattern(t)
        {
        }

        Basic:: Basic(const Basic &other) throw() : Pattern(other)
        {
        }


       bool Basic::accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
            Char *ch = source.get();
            if(!ch)
            {
                return false;
            }
            else
            {
                if( isValid(ch->code) )
                {
                    token << ch;
                    return true;
                }
                else
                {
                    source.unget(ch);
                    return false;
                }
            }
        }

        bool Basic:: feeble() const throw()
        {
            return false;
        }

        void Basic:: optimize() throw()
        {
            
        }
    }
}
