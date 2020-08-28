#include "y/jive/pattern/logic/or.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Or);

        Or:: ~Or() throw() {}

        Or::  Or() throw() : OutOfOrder(UUID)
        {
            Y_PATTERN_SELF(Or);
        }

        Or::  Or(const Or &_) throw() : OutOfOrder(_)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(Or);
        }

        Pattern *Or:: clone() const
        {
            return new Or(*this);
        }

        Or     * Or:: Create()
        {
            return new Or();
        }


        const char *Or:: className() const throw()
        {
            return CLID;
        }



        void Or:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"||\",shape=egg]");
            vizLink(fp);
        }

        void Or:: start(FirstChars &fc) const
        {
            assert(0==fc.size());
            for(const Pattern *op=head;op;op=op->next)
            {
                FirstChars sub;
                op->start(sub);
                fc.merge(sub);
            }
        }

        bool Or:: feeble() const throw()
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                if(op->feeble()) return true;
            }
            return false;
        }

        bool Or:: accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
            for(const Pattern *op=head;op;op=op->next)
            {
                if(op->accept(token,source))
                {
                    return true;
                }
            }
            return false;
        }


    }
}
