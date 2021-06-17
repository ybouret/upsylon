
#ifndef Y_JIVE_LANG_WILDCARD_INCLUDED
#define Y_JIVE_LANG_WILDCARD_INCLUDED 1

#include "y/jive/language/axiom/internal.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! Wildcard base class
            //
            //__________________________________________________________________
            class Wildcard : public Internal
            {
            public:
                virtual ~Wildcard() throw(); //!< cleanup
                const Axiom &axiom;          //!< axiom to handle

            protected:
                //! setup
                template <typename ID> inline
                explicit Wildcard(const ID &i, const uint32_t t, const Axiom &a) :
                Internal(i,t),
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
