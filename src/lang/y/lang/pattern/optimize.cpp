
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"

namespace upsylon
{
    namespace Lang
    {

        template <typename PAT>
        static inline Pattern * __Optimize( Pattern *q ) throw()
        {
            PAT           *p   = static_cast<PAT *>(q->priv);
            Pattern::List &ops = p->operands;
            if(1==ops.size)
            {
                Pattern *one = ops.pop_back();
                delete p;
                return Pattern::Optimize(one);
            }
            else
            {
                Pattern::List tmp;
                while(ops.size)
                {
                    Pattern *sub = Pattern::Optimize(ops.pop_front());
                    if( PAT::UUID == sub->uuid )
                    {
                        PAT *fusion = static_cast<PAT *>(sub->priv);
                        tmp.merge_back(fusion->operands);
                        delete sub;
                    }
                    else
                    {
                        tmp.push_back(sub);
                    }
                }
                ops.swap_with(tmp);
                return p;
            }
        }

        template <typename PAT>
        static inline Pattern * __OptimizeJoker( Pattern *p ) throw()
        {
            static_cast<PAT *>(p->priv)->optimize();
            return p;
        }

        Pattern * Pattern::Optimize( Pattern *p ) throw()
        {
            assert(p);

            switch(p->uuid)
            {
                case AND::UUID:
                    return __Optimize<AND>(p);

                case OR::UUID:
                    p = __Optimize<OR>(p);
                    if(OR::UUID==p->uuid)
                    {
                        // still a OR
                        NoMultiple(static_cast<OR *>(p->priv)->operands);
                    }
                    return p;

                case NONE::UUID: {
                    Pattern::List &ops = static_cast<NONE *>(p->priv)->operands;
                    Pattern::List  tmp;
                    while( ops.size ) tmp.push_back( Optimize( ops.pop_front() ) );
                    ops.swap_with(tmp);
                } break;

                case Optional  ::UUID: return __OptimizeJoker<Optional>(p);
                case Repeating ::UUID: return __OptimizeJoker<Repeating>(p);
                case Counting  ::UUID: return __OptimizeJoker<Counting>(p);

                default:
                    break;
            }
            return p;
        }
    }

}

