
#include "y/jargon/pattern/all.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Jargon {


        static inline
        Pattern *__ignore_case(Logical *target, const Logical *source )
        {
            auto_ptr<Pattern> guard(target);
            for(const Pattern *p = source->head; p; p=p->next)
            {
                target->push_back( p->ignore_case() );
            }
            return  guard.yield();
        }


        static inline
        void __ignore_case(const uint8_t code, Logical &target )
        {
            target.add( tolower(code) );
            target.add( toupper(code) );
        }

        Pattern *Pattern:: ignore_case() const
        {
            switch(uuid)
            {
                    //----------------------------------------------------------
                    //
                    // basic
                    //
                    //----------------------------------------------------------
                case Any1::UUID:
                    return clone();

                case Single::UUID: {
                    auto_ptr<Logical> p = OR::Create();
                    const Single     *s = static_cast<const Single *>(self);
                    __ignore_case(s->code,*p);
                    PairwiseMerge(*p);
                    return Optimize( p.yield() );
                }

                case Excluded::UUID: {
                    auto_ptr<Logical> p = NONE::Create();
                    const Single     *s = static_cast<const Single *>(self);
                    __ignore_case(s->code,*p);
                    PairwiseMerge(*p);
                    return Optimize( p.yield() );
                }

                case Range::UUID: {
                    auto_ptr<Logical> p = OR::Create();
                    const Range      *r = static_cast<const Range *>(self);
                    const int         lo = r->lower;
                    const int         up = r->upper;
                    for(int ch=lo;ch<=up;++ch)
                    {
                        __ignore_case(ch,*p);
                    }
                    PairwiseMerge(*p);
                    return Optimize(p.yield());
                }
                    //----------------------------------------------------------
                    //
                    // logical
                    //
                    //----------------------------------------------------------
#define Y_JARGON_IGNORE_CASE(TYPE)  case TYPE::  UUID: return Optimize( __ignore_case( TYPE::Create(), static_cast<const TYPE *>(self) ) )

                    Y_JARGON_IGNORE_CASE(AND);
                    Y_JARGON_IGNORE_CASE(OR);
                    Y_JARGON_IGNORE_CASE(NONE);


                    //----------------------------------------------------------
                    //
                    // joker
                    //
                    //----------------------------------------------------------
                case Optional::  UUID: return Optional::Create( static_cast<Optional *>(self)->motif->ignore_case() );
                case Repeating:: UUID: {
                    const Repeating *p = static_cast<Repeating *>(self);
                    return Repeating::Create(p->motif->ignore_case(), p->minimalCount);
                }
                case Counting:: UUID: {
                    const  Counting *p = static_cast<Counting *>(self);
                    return Counting::Create(p->motif->ignore_case(), p->minimalCount, p->maximalCount);
                }

                default:
                    break;
            }
            throw exception("Pattern::ignore_case(Unexpected [%s])", fourcc_(uuid));
        }

    }
}

