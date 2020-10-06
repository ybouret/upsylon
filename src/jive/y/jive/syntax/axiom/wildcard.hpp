
//! \file
#ifndef Y_JIVE_SYNTAX_WILDCARD_INCLUDED
#define Y_JIVE_SYNTAX_WILDCARD_INCLUDED 1

#include "y/jive/syntax/axiom/internal.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! Wildcard base class for a pesistent Axiom
            //
            //__________________________________________________________________
            class Wildcard : public Internal
            {
            public:
                //! cleanup
                virtual ~Wildcard() throw();


            protected:
                //! setup
                template <typename ID>
                explicit Wildcard(const ID      &i,
                                  const Axiom    &a,
                                  const uint32_t t) :
                Internal(i,t),
                axiom(a)
                {
                }

                const Axiom &axiom; //!< tested axiom

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
            };


        }

    }

}

#endif
