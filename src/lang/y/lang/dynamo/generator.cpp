
#include "y/lang/dynamo/generator.hpp"

namespace upsylon
{

    namespace Lang
    {

        DynamoGenerator:: DynamoGenerator() :
        parser(0)
        {
        }

        DynamoGenerator:: ~DynamoGenerator() throw()
        {

        }

        Syntax::Parser * DynamoGenerator:: create( Node *dynamo )
        {
            assert(dynamo);
            parser = 0;


            return parser.yield();
        }
    }

}
