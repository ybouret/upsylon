//! \file

#ifndef Y_JSON_PARSER_INCLUDED
#define Y_JSON_PARSER_INCLUDED 1

#include "y/jive/parser.hpp"

namespace upsylon
{

    namespace JSON
    {
        //______________________________________________________________________
        //
        //
        //! JSON grammar and parser
        //
        //______________________________________________________________________
        class Parser : public Jive::Parser
        {
        public:
            explicit Parser();          //!< setup grammar
            virtual ~Parser() throw();  //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif

