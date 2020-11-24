//! \file

#ifndef Y_JSON_PARSER_INCLUDED
#define Y_JSON_PARSER_INCLUDED 1

#include "y/jive/parser.hpp"

namespace upsylon
{

    namespace JSON
    {

        class Parser : public Jive::Parser
        {
        public:
            explicit Parser();
            virtual ~Parser() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif

