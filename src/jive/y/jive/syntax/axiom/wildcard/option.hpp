
//! \file
#ifndef Y_JIVE_SYNTAX_OPTION_INCLUDED
#define Y_JIVE_SYNTAX_OPTION_INCLUDED 1

#include "y/jive/syntax/axiom/wildcard.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! An optional Axiom
            //
            //__________________________________________________________________
            class Option : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('O','P','T',' '); //!< UUID

                //! cleanup
                virtual ~Option() throw();

                //! setup
                template <typename ID> inline
                explicit Option(const ID    &i,
                                const Axiom &a) :
                Wildcard(i,a,UUID)
                {
                    Y_JIVE_AXIOM(Option);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };


        }

    }

}

#endif
