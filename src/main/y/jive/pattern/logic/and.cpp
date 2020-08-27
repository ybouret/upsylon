

#include "y/jive/pattern/logic/and.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(And);

        And:: ~And() throw() {}

        And::  And() throw() : Sequential(UUID)
        {
            Y_PATTERN_SELF(And);
        }

        And::  And(const And &_) throw() : Sequential(_)
        {
            assert(UUID==uuid);
            Y_PATTERN_SELF(And);
        }

        Pattern *And:: clone() const
        {
            return new And(*this);
        }

        And     * And:: Create()
        {
            return new And();
        }


        const char *And:: className() const throw()
        {
            return CLID;
        }



        void And:: vizCore(ios::ostream &fp) const
        {
            endl(fp << "[label=\"&&\",shape=house]");
            vizLink(fp);
        }

        void And:: start(FirstChars &fc) const
        {
            assert(0==fc.size());
            for(const Pattern *op=operands.head;op;op=op->next)
            {
                FirstChars sub;
                op->start(sub);
                fc.merge(sub);
                if(op->strong())
                    return;
            }
        }

        bool And:: feeble() const throw()
        {
            for(const Pattern *op=operands.head;op;op=op->next)
            {
                if(op->strong()) return false;
            }
            return true;
        }

        bool And:: accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
            Token cat;
            for(const Pattern *op=operands.head;op;op=op->next)
            {
                Token tmp;
                if(op->accept(tmp,source))
                {
                    cat.merge_back(tmp);
                }
                else
                {
                    assert(0==tmp.size);
                    source.unget(cat);
                    return false;
                }
            }
            token.swap_with(cat);
            return true;
        }


    }
}
