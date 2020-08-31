

#include "y/jive/pattern/all.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Jive
    {

        namespace {

            template <typename T> Pattern * __ign(const T *p);


            template <> inline Pattern * __ign<Optional>(const Optional *p)
            {
                return Optional::Create( Pattern::IgnoreCase(p->motif) );
            }

            template <> inline Pattern * __ign<Repeating>(const Repeating *p)
            {
                return Repeating::Create( Pattern::IgnoreCase(p->motif), p->minCount );
            }

            template <> inline Pattern * __ign<Counting>(const Counting *p)
            {
                return Counting::Create( Pattern::IgnoreCase(p->motif), p->minCount, p->maxCount );
            }

            static inline void fill_ign(Operands &target, const Operands &source)
            {
                for(const Pattern *op=source.head;op;op=op->next)
                {
                    //target.push_back( op->clone() );
                    target.push_back( Pattern::IgnoreCase(op) );
                }
            }

            template <typename T> Pattern * __ign_logic(const T *p)
            {
                auto_ptr<T> q = T::Create();
                fill_ign( *q, *p);
                return Pattern::Optimize( q.yield() );
            }



        }

#define Y_JIVE_IGN_JOKER(TYPE) case TYPE::UUID: return __ign<TYPE>( p->as<TYPE>() )
#define Y_JIVE_IGN_LOGIC(TYPE) case TYPE::UUID: return __ign_logic<TYPE>( p->as<TYPE>() )

        Pattern * Pattern:: IgnoreCase(const Pattern *p)
        {
            assert(p);
            switch(p->uuid)
            {
                case Any   :: UUID: return p->clone();

                case Single:: UUID: {
                    auto_ptr<Or> q = Or::Create();
                    const char   C = p->as<Single>()->code;
                    q->add( char(tolower(C)) );
                    q->add( char(toupper(C)) );
                    return Pattern::Optimize(q.yield()); }

                case Exclude:: UUID: {
                    auto_ptr<None> q = None::Create();
                    const char     C = p->as<Exclude>()->code;
                    q->add( char(tolower(C)) );
                    q->add( char(toupper(C)) );
                    return Pattern::Optimize(q.yield()); }

                case Range:: UUID: {
                    auto_ptr<Or> q = Or::Create();
                    const int lo = p->as<Range>()->lower;
                    const int hi = p->as<Range>()->upper;
                    for(int code=lo;code<=hi;++code)
                    {
                        q->add( char(tolower(code)) );
                        q->add( char(toupper(code)) );
                    }
                    return Pattern::Optimize(q.yield()); }

                    Y_JIVE_IGN_LOGIC(And);
                    Y_JIVE_IGN_LOGIC(Or);
                    Y_JIVE_IGN_LOGIC(None);

                    Y_JIVE_IGN_JOKER(Optional);
                    Y_JIVE_IGN_JOKER(Repeating);
                    Y_JIVE_IGN_JOKER(Counting);

                default:
                    break;
            }
            throw exception("Jive::Pattern::IgnoreCase(unknown uuid=[%s])", fourcc_(p->uuid) );
        }


    }

}
