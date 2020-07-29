
#include "y/jargon/pattern/all.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Jargon {

        namespace {

            static inline
            Pattern *new_ignore_case(Logical *target, const Logical *source )
            {
                auto_ptr<Pattern> guard(target);
                for(const Pattern *p = source->head; p; p=p->next)
                {
                    target->push_back( p->ignore_case() );
                }
                return  guard.yield();
            }

            static const uint8_t a = 'a';
            static const uint8_t z = 'z';

            static const uint8_t A = 'A';
            static const uint8_t Z = 'Z';

            static inline
            bool __has_case(const uint8_t code )
            {
                return (code>=a&&code<=z) || (code>=A&&code<=Z);
            }

            static inline
            void add_ignore_case(const uint8_t code, Logical &target )
            {

                if(code>=a&&code<=z)
                {
                    target.add(code);
                    target.add(A+(code-a));
                }
                else if(code>=A&&code<=Z)
                {
                    target.add(code);
                    target.add(a+(code-A));
                }
                else
                {
                    target.add(code);
                }
            }

            static inline
            Pattern *new_ignore_case( Logical *target, const uint8_t ch )
            {
                auto_ptr<Pattern> guard(target);
                add_ignore_case(ch,*target);
                Pattern::PairwiseMerge(*target);
                return Pattern::Optimize(guard.yield());
            }

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
                case Any1::UUID: return clone();

                case Single::UUID: {
                    const Single     *s  = static_cast<const Single *>(self);
                    const uint8_t     c = s->code;
                    return (__has_case(c)) ? new_ignore_case(OR::Create(),c) : clone();
                }

                case Excluded::UUID: {
                    const Single     *s = static_cast<const Single *>(self);
                    const uint8_t     c = s->code;
                    return (__has_case(c)) ? new_ignore_case(NONE::Create(),c) : clone();
                }

                case Range::UUID: {
                    auto_ptr<Logical> p  = OR::Create();
                    Logical          &l  = *p;
                    const Range      *r  = static_cast<const Range *>(self);
                    const int         lo = r->lower;
                    const int         up = r->upper;
                    for(int ch=lo;ch<=up;++ch)
                    {
                        add_ignore_case(ch,l);
                    }
                    PairwiseMerge(l);
                    return Optimize(p.yield());
                }
                    //----------------------------------------------------------
                    //
                    // logical
                    //
                    //----------------------------------------------------------
#define Y_JARGON_IGNORE_CASE(TYPE)  case TYPE:: UUID: return Optimize( new_ignore_case( TYPE::Create(), static_cast<const TYPE *>(self) ) )

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

