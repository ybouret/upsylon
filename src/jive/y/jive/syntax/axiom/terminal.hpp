
//! \file
#ifndef Y_JIVE_SYNTAX_TERMINAL_INCLUDED
#define Y_JIVE_SYNTAX_TERMINAL_INCLUDED 1

#include "y/jive/syntax/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! a terminal Axiom
            /**
             - accept lexeme with *label==*name
             - reject if EOS of different label
             */
            //
            //__________________________________________________________________
            class Terminal : public Axiom
            {
            public:
                //! identifier
                static const uint32_t UUID = TermUUID;

                //! cleanup
                virtual ~Terminal() throw();

                //! setup
                template <typename ID> inline
                explicit Terminal(const ID &id) :
                Axiom(UUID,Tags::Make(id))
                {
                    Y_JIVE_AXIOM(Terminal);
                }

                Y_JIVE_AXIOM_ATTACH_DECL();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };

        }

    }

}

#endif
