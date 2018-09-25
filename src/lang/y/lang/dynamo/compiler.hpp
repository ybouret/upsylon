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
            arc_ptr<DynamoParser>   parser; //!< shared parser

            explicit DynamoCompiler();                      //!< initialize
            virtual ~DynamoCompiler() throw();              //!< destructor
            int   depth;
            Node *process( Module *module );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
            void processCommands( Node &dynamo, const string &origin );
            Node *format( Module *module );

        };
    }
}

#endif
