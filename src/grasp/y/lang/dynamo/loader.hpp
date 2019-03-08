
//! \file
#ifndef Y_LANG_DYNAMO_LOADER_INCLUDED
#define Y_LANG_DYNAMO_LOADER_INCLUDED 1

#include "y/lang/dynamo/parser.hpp"

namespace upsylon
{
    namespace Lang
    {

        class DynamoLoader : public DynamoParser
        {
        public:
            explicit DynamoLoader();
            virtual ~DynamoLoader() throw();

            //! load grammar node and perform includes
            Syntax::Node * load( Module *m );


            Syntax::Node * getCmdArgs( Syntax::Node &cmd, string &cmdName ) const;

        private:
            Y_DISABLE_COPY(DynamoLoader);
            void checkIncludes(Syntax::Node &node, const Module &currentModule);
        };
        
    }

}
#endif
