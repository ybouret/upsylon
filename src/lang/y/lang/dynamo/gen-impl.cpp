#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        void DynamoGenerator:: implRule( const DynamoNode &dynamo )
        {
            static const char fn[] = "DynamoGenerator.implRule";

            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( parser.is_valid() );

            if(dynamo.name!="dynamo")       throw exception("%s(expecting <dynamo> and not <%s>)",fn,*(dynamo.name));
            if(dynamo.type!=DynamoInternal) throw exception("%s(<dynamo> is not internal)",fn);

            const DynamoList     &self = dynamo.children();
            for(const DynamoNode *node = self.head; node; node=node->next )
            {

            }

        }
    }

}


