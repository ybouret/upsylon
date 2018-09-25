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
            typedef Syntax::Node    Node;      //!< alias
            DynamoParser            parser;    //!< per level parser
            int                     depth;     //!< current depth

            explicit DynamoCompiler();         //!< initialize
            virtual ~DynamoCompiler() throw(); //!< destructor

            Node *process( Module *module );   //!< recursively parse grammar


        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);

            void processCommands( Node &dynamo, const string &origin );
            Node *format( Module *module );

        };
    }
}

#endif
