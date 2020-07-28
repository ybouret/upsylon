
#include "y/jargon/pattern/all.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Jargon {


        static inline
        Pattern *_ignore_case(Logical *target, const Logical *source )
        {
            auto_ptr<Pattern> guard(target);
            for(const Pattern *p = source->head; p; p=p->next)
            {
                target->push_back( p->ignore_case() );
            }
            return  guard.yield();
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
                    p->add( tolower(s->code) );
                    p->add( toupper(s->code) );
                    PairwiseMerge(*p);
                    return Optimize( p.yield() );
                }

                case Excluded::UUID: {
                    auto_ptr<Logical> p = NONE::Create();
                    const Single     *s = static_cast<const Single *>(self);
                    p->add( tolower(s->code) );
                    p->add( toupper(s->code) );
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
                        p->add( tolower(ch) );
                        p->add( toupper(ch) );
                    }
                    PairwiseMerge(*p);
                    return Optimize(p.yield());
                }
                    //----------------------------------------------------------
                    //
                    // logical
                    //----------------------------------------------------------
                case AND::  UUID: return Optimize( _ignore_case( AND::Create(), static_cast<const AND *>(self) ) );
                case OR::   UUID: return Optimize( _ignore_case( OR::Create(),   static_cast<const OR *>(self) ) );
                case NONE:: UUID: return Optimize( _ignore_case( NONE::Create(), static_cast<const NONE *>(self) ) );

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

