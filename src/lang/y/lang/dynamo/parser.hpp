//! \file
#ifndef Y_LANG_DYNAMO_PARSER_INCLUDED
#define Y_LANG_DYNAMO_PARSER_INCLUDED 1

#include "y/lang/compiler.hpp"

namespace upsylon {

    namespace Lang
    {

        //! Dynamic Grammar Parser
        class DynamoParser : public Syntax::Parser
        {
        public:
            //! define the generator parser
            explicit DynamoParser();

            //! destructor
            virtual ~DynamoParser() throw();

            int includeDepth;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoParser);
        };
        
    };

}

#endif

