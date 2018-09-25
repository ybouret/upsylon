//! \file

#ifndef Y_LANG_DYNAMO_GENERATOR_INCLUDED
#define Y_LANG_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/syntax/parser.hpp"

namespace upsylon
{

    namespace Lang
    {

        //! Generate a parser from a Dynamo compiled AST
        class DynamoGenerator
        {
        public:
            typedef Syntax::Node Node;

            explicit DynamoGenerator();
            virtual ~DynamoGenerator() throw();

            Syntax::Parser *create( Node *dynamo );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
            auto_ptr<Syntax::Parser> parser;
        };


    }

}

#endif

