//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! for building set of rules
            class Grammar
            {
            public:
                explicit Grammar();          //!< initialize
                virtual ~Grammar() throw();  //!< desctructor

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Rule::List rules;
            };
        }
    }
}

#endif

