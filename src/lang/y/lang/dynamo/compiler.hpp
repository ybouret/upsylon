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
            typedef Syntax::Node    Node;   //!< alias
            Syntax::Parser::Pointer parser; //!< shared parser

            explicit DynamoCompiler();                      //!< initialize
            DynamoCompiler(const DynamoCompiler &) throw(); //!< shared copy
            virtual ~DynamoCompiler() throw();              //!< destructor

            Node *format( Module *module );

        private:
            Y_DISABLE_ASSIGN(DynamoCompiler);
            void processCommands( Node &dynamo );
        };
    }
}

#endif
