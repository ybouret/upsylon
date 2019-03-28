//! \file

#ifndef Y_DYNAMO_MANAGER_INCLUDED
#define Y_DYNAMO_MANAGER_INCLUDED 1

#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/loader.hpp"


namespace upsylon
{
    namespace Lang
    {

        //! singleton to create parsers
        class DynamoManager : public singleton<DynamoManager>, public DynamoGenerator
        {
        public:
            //! loader on-the-fly creation
            DynamoLoader   & loader();
            
            //! compiler on-the-fly creation
            DynamoCompiler & compiler();
            
            //! load DynamoNode and build parser
            Syntax::Parser *link( Module *dynamoModule, DynamoSymbols *symbols=NULL);
            
            //! load SyntaxNode, compiler and build
            Syntax::Parser *loadAndLink( Module *grammarModule, DynamoSymbols *symbols=NULL );
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoManager);
            friend class singleton<DynamoManager>;
            explicit DynamoManager() throw();
            virtual ~DynamoManager() throw();
            static const at_exit::longevity life_time = -13;

            auto_ptr<DynamoLoader>    __loader;
            auto_ptr<DynamoCompiler>  __compiler;
        };

    }
}

#endif

