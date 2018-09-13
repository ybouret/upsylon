//!\file
#ifndef Y_LANG_SYNTAX_RULE_INCLUDED
#define Y_LANG_SYNTAX_RULE_INCLUDED

#include "y/lang/syntax/node.hpp"
#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Rule : public Object
            {
            public:
                typedef core::list_of_cpp<Rule> List;

                const uint32_t uuid;
                Rule          *next;
                Rule          *prev;

                virtual ~Rule()  throw();

            protected:
                explicit Rule(const uint32_t t) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

        }

    }

}

#endif

