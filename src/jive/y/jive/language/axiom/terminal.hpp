
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
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('T', 'E', 'R', 'M'); //!< UUID

                //! type of terminal
                enum Type
                {
                    Standard, //!< regular expression
                    Univocal, //!< univocal (regular) expression
                    Division  //!< whatever, division only
                };

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                Y_LANG_AXIOM_DECL();

                virtual ~Terminal() throw(); //!< cleanup

                //! setup
                template <typename ID> inline
                explicit Terminal(const ID  &i,
                                  const Type t) :
                Axiom(i, UUID), type(t) { I_am<Terminal>(); }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Type type; //!< semantic

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}

#endif
