#include "y/jive/pattern/logic/or.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(Or);

        Or:: ~Or() throw() {}

        Or::  Or() throw() : Reworkable(UUID)
        {
            Y_PATTERN_SELF(Or);
        }

        Or::  Or(const Or &_) throw() : Reworkable(_)
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

        void Or:: start(Leading &fc) const
        {
            assert(0==fc.size);
            for(const Pattern *op=head;op;op=op->next)
            {
                Leading sub;
                op->start(sub);
                fc.include(sub);
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
            bool accepted = false;
            for(const Pattern *op=head;op;op=op->next)
            {
                if(op->accept(token,source))
                {
                    if(token.size>0)
                    {
                        return true;
                    }
                    else
                    {
                        assert(op->feeble());
                        accepted = true;
                    }
                }
            }
            return  accepted;
        }

        void Or:: optimize() throw()
        {
            applyOptimize();
            mergeSameUUID();
            rework();
        }

        void Or::express(ios::ostream &fp) const
        {
            if(size>1)
            {
                fp << '(';
            }
            for(const Pattern *op=head;op;)
            {
                op->express(fp);
                op=op->next;
                if(op)
                {
                    fp << '|';
                    continue;
                }
                else
                {
                    break;
                }
            }
            if(size>1)
            {
                fp << ')';
            }
        }

        bool Or:: univocal() const throw()
        {
            return 1==size && (head->univocal());
        }
    }

}

#include "y/jive/pattern/basic/rework.hpp"
namespace upsylon {

    namespace Jive
    {

        void Or:: rework() throw()
        {
            Operands strongList;
            Operands feebleList;
            while(size)
            {
                if(head->feeble())
                {
                    feebleList.push_back( pop_front() );
                }
                else
                {
                    strongList.push_back( pop_front() );
                }
            }

            Rework::Compact(strongList);
            swap_with(strongList);
            merge_back(feebleList);
        }
    }
}


#include "y/jive/pattern/basic/single.hpp"
#include "y/ptr/auto.hpp"
namespace upsylon
{

    namespace Jive
    {

        Pattern * Logical:: Among(const char *buffer, size_t buflen)
        {
            assert(!(NULL==buffer&&buflen>0));

            auto_ptr<Or> p = Or::Create();
            while(buflen-- > 0 )
            {
                p->push_back( Single::Create(*(buffer++)) );
            }
            return Optimize( p.yield() );
        }


        Pattern * Logical:: Among(const string &s)
        {
            return Among(*s,s.size());
        }

        Pattern * Logical:: Among( const char *s )
        {
            return Among(s,length_of(s));
        }
    }
}
