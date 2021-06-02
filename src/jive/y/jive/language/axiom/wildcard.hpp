
#ifndef Y_JIVE_LANG_WILDCARD_INCLUDED
#define Y_JIVE_LANG_WILDCARD_INCLUDED 1

#include "y/jive/language/axiom.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class Wildcard : public Axiom
            {
            public:
                virtual ~Wildcard() throw();
                const Axiom &axiom;

            protected:
                template <typename ID> inline
                explicit Wildcard(const ID &i, const uint32_t t, const Axiom &a) :
                Axiom(i,t),
                axiom(a)
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
            };
        }

    }

}

#endif
