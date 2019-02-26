
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/basic.hpp"

#include "y/exception.hpp"


namespace upsylon
{
    namespace Lang
    {

        static inline
        void __fill( Pattern::List &ops, const char *s, const size_t n )
        {
            assert(s);
            assert(n);
            for(size_t i=0;i<n;++i)
            {
                ops.push_back( new Single(s[i]) );
            }
        }


        Pattern * Logical:: Equal(const string &s)
        {
            auto_ptr<Logical> p = new AND();
            const size_t      n = s.size();
            if(n<=0) throw exception("Lang::Logical::Equal(NULL)");
            __fill(p->operands,*s,n);
            return p.yield();
        }

        Pattern * Logical::Equal(const char *s) { const string _ = s; return Equal(_); }

        Pattern * Logical:: Among(const string &s)
        {
            auto_ptr<Logical> p = new OR();
            const size_t      n = s.size();
            if(n<=0) throw exception("Lang::Logical::Among(NULL)");
            __fill(p->operands,*s,n);
            return p.yield();
        }

        Pattern * Logical:: Among(const char *s) { const string _ = s; return Among(_); }

    }

}


