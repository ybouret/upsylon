
//! \file

#ifndef Y_JIVE_LANG_TERMINAL_INCLUDED
#define Y_JIVE_LANG_TERMINAL_INCLUDED 1

#include "y/jive/language/axiom.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! Terminal
            //
            //__________________________________________________________________
            class Terminal : public Axiom
            {
            public:
                static const uint32_t UUID = Y_FOURCC('T', 'E', 'R', 'M');

                virtual ~Terminal() throw();
                
                template <typename ID> inline
                explicit Terminal( const ID &id ) : Axiom( id, UUID) { }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}

#endif
