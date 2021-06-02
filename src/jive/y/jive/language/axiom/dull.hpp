
//! \file

#ifndef Y_JIVE_LANG_DULL_INCLUDED
#define Y_JIVE_LANG_DULL_INCLUDED 1

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
            //! just a dull internal node to test
            //
            //__________________________________________________________________
            class Dull : public Internal
            {
            public:
                template <typename ID> inline
                explicit              Dull(const ID &i) : Internal(i,UUID) {} //!< setup
                virtual              ~Dull() throw();                         //!< cleanup
                static const uint32_t UUID   = Y_FOURCC('D','U','L','L');     //!< UUID
                Y_LANG_AXIOM_DECL(); //!< false

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Dull);
            };
        }
    }
}

#endif
