
#include "y/alchemy/limit.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Limit:: Limit() throw() : on(false), xi(0) {}
        Limit:: Limit(const Limit &other) throw() :
        on(other.on),
        xi(other.xi)
        {
        }

        Limit:: ~Limit() throw()
        {
            reset();
        }


        void Limit:: reset() throw() { on=false; xi=0; }
    }

    namespace Alchemy
    {
        Limits:: Limits() throw() :
        lower(),
        upper(),
        state(Unbounded)
        {
            
        }

        Limits:: ~Limits() throw()
        {
        }

        Limits:: Limits(const Limits &other) throw() :
        lower(other.lower),
        upper(other.upper),
        state(other.state)
        {

        }

        void Limits:: reset()  throw()
        {
            lower.reset();
            upper.reset();
            state=Unbounded;
        }

        bool Limits:: mobility() throw()
        {
            if(lower.on)
            {
                if(upper.on)
                {
                    if(lower.xi<=upper.xi)
                    {
                        state = WithRange;
                    }
                    else
                    {
                        state = Exclusive;
                    }
                }
                else
                {
                    assert(!upper.on);
                    state = LowerOnly;
                }
            }
            else
            {
                assert(!lower.on);
                if(upper.on)
                {
                    state = UpperOnly;
                }
                else
                {
                    assert(!upper.on);
                    state = Unbounded;
                }
            }
            return state != Exclusive;
        }

    }

    namespace Alchemy
    {
        Limits::Array:: Array(size_t n) :  Array_(n,as_capacity)
        {
            while(n-- > 0)
            {
                const Limits _;
                push_back_(_);
            }
        }

        Limits:: Array:: ~Array() throw()
        {
        }

    }

}
