//! \file
#ifndef Y_LANG_DYNAMO_PARSER_INCLUDED
#define Y_LANG_DYNAMO_PARSER_INCLUDED 1

#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        class DynamoParser : public Syntax::Parser
        {
        public:
            explicit DynamoParser();
            virtual ~DynamoParser() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoParser);
        };
    }
}

#endif

