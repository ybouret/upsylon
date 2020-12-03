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
            //! build grammar to achieve parser
            //
            //__________________________________________________________________
            class Parser : public Jive::Parser
            {
            public:
                //! setup grammar
                explicit Parser();
                
                //! cleanup
                virtual ~Parser() throw();

                static const char Include[];
                static const char bStringID[];
                static const char jStringID[];
                static const char rStringID[];

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }
    }

}

#endif

