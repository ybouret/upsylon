//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/loader.hpp"
#include "y/lang/syntax/analyzer.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! used to build in intermediate tree
        class DynamoNode : public core::inode<DynamoNode>
        {
        public:
            explicit DynamoNode( const Tag &mid );
            virtual ~DynamoNode() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoNode);
        };
    
        //! build an intermediate tree
        class DynamoCompiler : public DynamoLoader, public Syntax::Analyzer
        {
        public:
            explicit DynamoCompiler();
            virtual ~DynamoCompiler() throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
        };
        
    }
}

#endif

