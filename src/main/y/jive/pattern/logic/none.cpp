
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

        void None:: start(Leading &fc) const
        {
            assert(0==fc.size);
            fc.complete();
            for(const Pattern *op=head;op;op=op->next)
            {
                Leading sub;
                op->start(sub);
                fc.exclude(sub);
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
                token.push(ch);
                return true;
            }
            else
            {
                return false;
            }
        }


        
        void None::express(ios::ostream &fp) const
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
            fp << '~';
        }

        bool None:: univocal() const throw()
        {
            return false;
        }

    }
}
#include "y/jive/pattern/basic/rework.hpp"
#include "y/sort/merge-list.hpp"

namespace upsylon {

    namespace Jive
    {

        static inline
        int byIncreasingEntropy(const Pattern *lhs, const Pattern * rhs, void *) throw()
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
                assert(NULL!=head);
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
            merge_list_of<Pattern>::sort(strongList,byIncreasingEntropy,NULL);
            swap_with(strongList);
            merge_back(feebleList);
        }
    }

}

#include "y/ptr/auto.hpp"
namespace upsylon
{

    namespace Jive
    {

        Pattern * Logical:: Avoid(const char *buffer, size_t buflen)
        {
            assert(!(NULL==buffer&&buflen>0));

            auto_ptr<None> p = None::Create();
            while(buflen-- > 0 )
            {
                p->push_back( Single::Create(*(buffer++)) );
            }
            return Optimize( p.yield() );
        }

        Pattern * Logical:: Avoid(const string &s)
        {
            return Avoid(*s,s.size());
        }

        Pattern * Logical:: Avoid( const char *s )
        {
            return Avoid(s,length_of(s));
        }
    }
}

#include "y/jive/pattern/logic/or.hpp"

namespace upsylon
{

    namespace Jive
    {

        void None:: optimize() throw()
        {
            // optimize all operands
            applyOptimize();

            // merge OR
            {
                Operands tmp;
                while(size)
                {
                    Pattern *op = pop_front();
                    if(Or::UUID==op->uuid)
                    {
                        tmp.merge_back( *( op->as<Or>() ) );
                        delete op;
                    }
                    else
                    {
                        tmp.push_back(op);
                    }
                }
                swap_with(tmp);
            }

            // rework
            rework();
        }

    }
}

