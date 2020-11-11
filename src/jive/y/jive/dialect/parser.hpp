//! \file

#ifndef Y_JIVE_DIALECT_PARSER_INCLUDED
#define Y_JIVE_DIALECT_PARSER_INCLUDED 1

#include "y/jive/parser.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {
            //__________________________________________________________________
            //
            //
            //! build grammar to achive parser
            //
            //__________________________________________________________________
            class Parser : public Jive::Parser
            {
            public:
                //! setup grammar
                explicit Parser();
                
                //! cleanup
                virtual ~Parser() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }
    }

}

#endif

