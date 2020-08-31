
#include "y/jive/pattern/logic/none.hpp"

namespace upsylon {

    namespace Jive
    {
        Y_PATTERN_CLID(None);

        None:: ~None() throw() {}

        None::  None() throw() : Reworkable(UUID)
        {
            Y_PATTERN_SELF(None);
        }

        None::  None(const None &_) throw() : Reworkable(_)
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
            for(const Pattern *op=head;op;op=op->next)
            {
                FirstChars sub;
                op->start(sub);
                fc.strip(sub);
            }
        }

        bool None:: feeble() const throw()
        {
            return false;
        }

        bool None:: accept(Y_PATTERN_ACCEPT_ARGS) const
        {
            assert(0==token.size);
            for(const Pattern *op=head;op;op=op->next)
            {
                Token tmp;
                if(op->accept(tmp,source))
                {
                    source.unget(tmp);
                    return false;
                }
            }
            Char *ch = source.get();
            if(ch)
            {
                token << ch;
                return true;
            }
            else
            {
                return false;
            }
        }

        void None:: optimize() throw()
        {
            applyOptimize();
            rework();
        }
        
        void None::express(ios::ostream &fp) const
        {
            if(size>1)
            {
                fp << '(';
            }
            for(const Pattern *op=head;;)
            {
                op->express(fp);
                op=op->next;
                if(op)
                {
                    fp << ',';
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
            fp << '~';
        }

    }
}
#include "y/jive/pattern/basic/rework.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    namespace Jive
    {

        static int byIncreasingEntropy(const Pattern *lhs, const Pattern * rhs, void *) throw()
        {
            const double L = lhs->entropy();
            const double R = rhs->entropy();
            return comparison::increasing(L,R);
        }

        void None:: rework() throw()
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
            merging<Pattern>::sort(strongList,byIncreasingEntropy,NULL);
            swap_with(strongList);
            merge_back(feebleList);
        }
    }

}
