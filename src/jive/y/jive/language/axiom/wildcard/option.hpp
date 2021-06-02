//! \file

#ifndef Y_JIVE_LANG_OPTION_INCLUDED
#define Y_JIVE_LANG_OPTION_INCLUDED 1

#include "y/jive/language/axiom/wildcard.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! make an optional axion
            //
            //__________________________________________________________________
            class Option : public Wildcard
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('O', 'P', 'T', 'N'); //!< UUID

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                template <typename ID> inline
                explicit Option(const ID &i, const Axiom &a) : Wildcard(i,UUID,a) { I_am<Option>(); }

                //! cleanup
                virtual ~Option() throw();

                Y_LANG_AXIOM_DECL();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);
            };

        }
    }
}

#endif
