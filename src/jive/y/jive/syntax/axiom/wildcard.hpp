
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



                const Axiom &axiom; //!< tested axiom


            protected:
                //! setup
                template <typename ID>
                explicit Wildcard(const ID      &i,
                                  const Axiom    &a,
                                  const uint32_t t) :
                Internal(i,t),
                axiom(a)
                {
                    a.called_by(*this);
                }

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
                Y_JIVE_AXIOM_ATTACH_DECL();
            };


        }

    }

}

#endif
