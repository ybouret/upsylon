//! \file
#ifndef Y_DYNAMO_GENERATOR_INCLUDED
#define Y_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        typedef memory::pooled DynamoMemory; //!< internal memory type


        //! generates a parser from a compiled tree
        class DynamoGenerator
        {
        public:
            typedef vector<const Tag,DynamoMemory> Modules; //!< alias
            virtual ~DynamoGenerator() throw();
            explicit DynamoGenerator();

            Syntax::Parser * build( DynamoNode &top );
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);


            auto_ptr<Syntax::Parser> parser;   //!< currently built parser
            const hashing::mperf     declH;    //!< "dynamo"
            Modules                  modules;  //!< stack of modules
            int                      level;    //!< depth

            void decl( DynamoNode &node );
            void declInternal( DynamoNode       &node );
            void declTerminal( const DynamoNode &node );
            void declModule( DynamoNode &dynamo );
            void declAlias( const DynamoNode &alias );

        public:
            bool verbose;
        };
    }

}

#endif

