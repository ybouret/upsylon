
#include "y/jive/pattern/logic/none.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(None);

        None:: ~None() throw() {}

        None::  None() throw() : OutOfOrder(UUID)
        {
            Y_PATTERN_SELF(None);
        }

        None::  None(const None &_) throw() : OutOfOrder(_)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(None);
        }

        Pattern *None:: clone() const
        {
            return new None(*this);
        }

        None     * None:: Create()
        {
            return new None();
        }


        const char *None:: className() const throw()
        {
            return CLID;
        }



        void None:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"!=\",shape=invtriangle]");
            vizLink(fp);
        }

        void None:: start(FirstChars &fc) const
        {
            assert(0==fc.size());
            fc.complete();
            for(const Pattern *op=operands.head;op;op=op->next)
            {
                FirstChars sub;
                op->start(sub);
                fc.strip(sub);
            }
        }

        bool None:: feeble() const throw()
        {
#if 0
            for(const Pattern *op=operands.head;op;op=op->next)
            {
                if(op->feeble()) return true;
            }
#endif
            return false;
        }

        bool None:: accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
#if 0
            for(const Pattern *op=operands.head;op;op=op->next)
            {
                if(op->accept(token,source))
                {
                    return true;
                }
            }
#endif
            return false;
        }


    }
}
