
#include "y/yap/prime/sprp.hpp"
#include "y/yap/library.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace yap
    {

        sprp:: ~sprp() throw()
        {
            aliasing::_(s) = 0;
        }

#define Y_SPRP_CTOR(x) \
n(x),   \
m(n-1), \
d(m),   \
s(0)

        void sprp::check() const
        {
            if(n.is_even())
            {
                throw exception("SPRP(even natural)");
            }
        }

        sprp:: sprp(const natural &x) :
        Y_SPRP_CTOR(x)
        {
            check();
            make();
        }

        sprp:: sprp(const number::utype x) :
        Y_SPRP_CTOR(x)
        {
            check();
            make();
        }

        void sprp:: make() throw()
        {
            while(d.is_even())
            {
                aliasing::_(d).shr(1);
                ++aliasing::_(s);
            }
            
        }

        bool sprp:: is(const number::utype u) const
        {
            const natural _(u);
            return is(_);
        }


        bool sprp:: is(const natural &a) const
        {
            static const library &apl = library::instance();
            static const natural  one = apl._1;

            if(one==natural::mod_exp(a,d,n))
            {
                return true;
            }
            else
            {
                natural e = d;
                size_t  r = s;
#ifndef NDEBUG
                size_t  count=0;
                assert(d<<count==e);
#endif
                if(m==natural::mod_exp(a,e,n))
                {
                    return true;
                }
                else
                {
                    while(--r>0)
                    {
                        e.shl(1);
#ifndef NDEBUG
                        ++count;
                        assert(count<s);
                        assert(d<<count==e);
#endif
                        if(m==natural::mod_exp(a,e,n))
                        {
                            return true;
                        }
                    }
                }

                return false;
            }

        }


        sprp:: sprp(const sprp &other) :
        n(other.n),
        m(other.m),
        d(other.d),
        s(other.s)
        {
        }
        


    }

}
