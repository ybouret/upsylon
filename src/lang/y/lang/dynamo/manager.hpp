//! \file

#ifndef Y_DYNAMO_MANAGER_INCLUDED
#define Y_DYNAMO_MANAGER_INCLUDED 1

#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/parser.hpp"


namespace upsylon
{
    namespace Lang
    {

        //! singleton to create parsers
        class DynamoManager : public singleton<DynamoManager>
        {
        public:
            //! loader on-the-fly creation
            DynamoParser   & loader();
            
            //! compiler on-the-fly creation
            DynamoCompiler & compiler();
            
            //! Generator on-the-fly creation
            DynamoGenerator & generator();
            
            //! load a compiled DynamoNode and build parser
            Syntax::Parser *link( Module *dynamoModule, DynamoSymbols *symbols=NULL);
            
            //! load SyntaxNode, compiler and build
            Syntax::Parser *loadAndLink( Module *grammarModule, DynamoSymbols *symbols=NULL );
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoManager);
            friend class singleton<DynamoManager>;
            explicit DynamoManager() throw();
            virtual ~DynamoManager() throw();
            static const at_exit::longevity life_time = -13;

            auto_ptr<DynamoParser>    __parser;
            auto_ptr<DynamoCompiler>  __compiler;
            auto_ptr<DynamoGenerator> __generator;
        };

    }
}

#endif

