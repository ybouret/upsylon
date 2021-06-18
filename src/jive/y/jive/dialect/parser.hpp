//! \file

#ifndef Y_JIVE_DIALECT_INCLUDED
#define Y_JIVE_DIALECT_INCLUDED 1

#include "y/jive/language/parser.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

            //__________________________________________________________________
            //
            //
            //! Dialect Parser
            //
            //__________________________________________________________________
            class Parser : public Language::Parser
            {
            public:
                explicit Parser();         //!< setup
                virtual ~Parser() throw(); //!< cleanup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }
    }
}


#endif

