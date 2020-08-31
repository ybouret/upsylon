
#include "y/jive/pattern/all.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {

        static inline bool __eq(const Range *lhs, const Range *rhs) throw()
        {
            return (lhs->lower == rhs->lower) && (lhs->upper==rhs->upper);
        }

        static inline bool __eq(const Optional *lhs, const Optional *rhs)
        {
            return Pattern::AreEqual( *(lhs->motif), *(rhs->motif) );
        }

        static inline bool __eq(const Repeating *lhs, const Repeating *rhs)
        {
            return (lhs->minCount==rhs->minCount) && Pattern::AreEqual( *(lhs->motif), *(rhs->motif) );
        }


        static inline bool __eq(const Counting *lhs, const Counting *rhs)
        {
            return (lhs->minCount==rhs->minCount) && (lhs->maxCount==rhs->maxCount) &&Pattern::AreEqual( *(lhs->motif), *(rhs->motif) );
        }

        static inline bool __eq(const Operands *lhs, const Operands *rhs)
        {
            if(lhs->size!=rhs->size)
            {
                return false;
            }
            else
            {
                size_t         n = lhs->size;
                const Pattern *L = lhs->head;
                const Pattern *R = rhs->head;
                while(n-- > 0 )
                {
                    assert(L);
                    assert(R);
                    if(!Pattern::AreEqual(*L,*R)) return false;
                    L=L->next;
                    R=R->next;
                }
                assert(!L);
                assert(!R);
                return true;
            }
        }


#define Y_JIVE_EQ(TYPE) case TYPE::UUID: return __eq( lhs.as<TYPE>(), rhs.as<TYPE>() )

        bool Pattern:: AreEqual(const Pattern &lhs, const Pattern &rhs)
        {
            if(lhs.uuid!=rhs.uuid)
            {
                return false;
            }
            else
            {
                switch(lhs.uuid)
                {
                    case Any::     UUID: return true;
                    case Single::  UUID: return lhs.as<Single>()->code  == rhs.as<Single>()->code;
                    case Exclude:: UUID: return lhs.as<Exclude>()->code == rhs.as<Exclude>()->code;
                        Y_JIVE_EQ(Range);
                        Y_JIVE_EQ(Optional);
                        Y_JIVE_EQ(Repeating);
                        Y_JIVE_EQ(Counting);
                        Y_JIVE_EQ(Or);
                        Y_JIVE_EQ(And);
                        Y_JIVE_EQ(None);
                    default:
                        break;
                }
                throw exception("Jive::Pattern::AreEqual(no uuid<%s>)", fourcc_(lhs.uuid) );
                
            }
        }
    }

}

