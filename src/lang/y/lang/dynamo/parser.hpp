//! \file
#ifndef Y_LANG_DYNAMO_PARSER_INCLUDED
#define Y_LANG_DYNAMO_PARSER_INCLUDED 1

#include "y/lang/compiler.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon {

    namespace Lang
    {

        //! Dynamic Grammar Parser
        class DynamoParser : public Syntax::Parser
        {
        public:
            static const int include = 0;
            //! define the generator parser
            explicit DynamoParser();

            //! destructor
            virtual ~DynamoParser() throw();
            
            hashing::mperf commands;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoParser);
        };
        
    };

}

#endif

