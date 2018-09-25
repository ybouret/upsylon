//! \file
#ifndef Y_LANG_DYNAMO_COMPILER_INCLUDED
#define Y_LANG_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/parser.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! post process the AST from DynamoParser
        class DynamoCompiler
        {
        public:
            Syntax::Parser::Pointer parser; //!< shared parser
            
            explicit DynamoCompiler();                      //!< initialize
            DynamoCompiler(const DynamoCompiler &) throw(); //!< shared copy
            virtual ~DynamoCompiler() throw();              //!< destructor

        private:
            Y_DISABLE_ASSIGN(DynamoCompiler);
        };
    }
}

#endif
