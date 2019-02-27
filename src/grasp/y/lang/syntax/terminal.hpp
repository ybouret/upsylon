//! \file
#ifndef Y_LANG_SYNTAX_TERMINAL_INCLUDED
#define Y_LANG_SYNTAX_TERMINAL_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            //! standard terminal matching a regular expression
            class Terminal :  public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('T','E','R','M');

                explicit Terminal(const string &n);
                virtual ~Terminal() throw();

                Y_LANG_SYNTAX_ACCEPT_PROTO();
                virtual const char *typeName() const throw();
                virtual bool        isHollow() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };


        }
    }
}


#endif

