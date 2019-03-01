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

            //! standard terminal matching a lexeme
            class Terminal :  public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< UUID

                explicit Terminal(const string &n); //!< setup
                virtual ~Terminal() throw();        //!< desctructor

                Y_LANG_SYNTAX_ACCEPT_PROTO();                 //!< accept a terminal with name==label
                virtual const char *typeName() const throw(); //!< "Terminal"
                virtual bool        isHollow() const throw(); //!< false
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };


        }
    }
}


#endif

