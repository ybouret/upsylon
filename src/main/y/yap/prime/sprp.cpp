
#include "y/yap/prime/sprp.hpp"
#include "y/yap/library.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include <cmath>

namespace upsylon
{

    namespace yap
    {

        sprp:: dnode::  dnode(const natural &value ) : natural(value), next(0) {}
        sprp:: dnode::  dnode(const dnode   &other ) : natural(other), next(0) {}
        sprp:: dnode:: ~dnode() throw()
        {
            assert(0==next);
        }

        sprp:: ~sprp() throw()
        {
            aliasing::_(s) = 0;
        }

#define Y_SPRP_CTOR(x) \
n(x),   \
m(n-1), \
l(),    \
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

        void sprp:: make()
        {
            dlist   &data = aliasing::_(l);

            // building first node: n=2^s * d
            natural &d    = *data.push_back( new dnode(m) );
            while(d.is_even())
            {
                aliasing::_(d).shr(1);
                ++aliasing::_(s);
            }


            // building following nodes
            for(size_t r=1;r<s;++r)
            {
                const natural e = (*data.tail) << 1;
                assert(d<<r==e);
                (void) data.push_back(new dnode(e) );
            }
            
        }

        bool sprp:: base(const number::utype u) const
        {
            const natural _(u);
            return base(_);
        }


        bool sprp:: base(const natural &a) const
        {
            static const library &apl = library::instance();
            static const natural  one = apl._1;

            const dnode   *d = l.head;
            const natural first = natural::mod_exp(a,*d,n);
            if(first==one||first==m)
            {
                return true;
            }
            else
            {
                for(d=d->next;d;d=d->next)
                {
                    if(m==natural::mod_exp(a,*d,n))
                    {
                        return true;
                    }
                }
                return false;
            }

        }


        sprp:: sprp(const sprp &other) :
        n(other.n),
        m(other.m),
        l(other.l),
        s(other.s)
        {
        }
        
        natural sprp:: end(const natural &n)
        {
            const double        dvalue = ceil( 2.0 * square_of(natural::log_of(n)) );
            const number::utype uvalue(dvalue);
            const natural       guess = uvalue;
            if(guess<=n)
            {
                return guess;
            }
            else
            {
                return n;
            }
        }

        natural sprp:: end(const number::utype u)
        {
            const natural _(u);
            return end(_);
        }

    }

}
