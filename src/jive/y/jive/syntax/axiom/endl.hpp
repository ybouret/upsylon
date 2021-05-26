
//! \file
#ifndef Y_JIVE_SYNTAX_ENDL_INCLUDED
#define Y_JIVE_SYNTAX_ENDL_INCLUDED 1

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
            //! an end-line axion
            /**
             - accept lexeme with *label==*name
             - reject if EOS or different label
             */
            //
            //__________________________________________________________________
            class EndLine : public Axiom
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________

                //! identifier
                static const uint32_t UUID = Y_FOURCC('E', 'N', 'D', 'L');

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~EndLine() throw();

                //! setup
                template <typename ID> inline
                explicit EndLine(const ID   &i) :
                Axiom(UUID,Tags::Make(i))
                {
                    Y_JIVE_AXIOM(EndLine);
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                Y_JIVE_AXIOM_ATTACH_DECL();
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(EndLine);
                Y_JIVE_AXIOM_ACCEPT_DECL();

            };

        }

    }

}

#endif
