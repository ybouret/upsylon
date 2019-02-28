//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Grammar : public Object
            {
            public:
                const Tag name; //!< shared tag

                explicit Grammar(const Tag &tid);
                virtual ~Grammar() throw();

                void setVerbose(const bool flag) throw(); //!< propagate flag to all rules

            private:
                Rule::List rules;
                bool       verbose;
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            };

        }
    }
}


#endif


