
#include "y/lang/dynamo/manager.hpp"


namespace upsylon
{
    namespace Lang
    {
        DynamoManager:: ~DynamoManager() throw()
        {
        }

        DynamoManager:: DynamoManager() throw() :
        gen(0),
        ld(0)
        {
        }


        DynamoGenerator & DynamoManager:: generator()
        {
            if(gen.is_empty())
            {
                gen = new DynamoGenerator();
            }
            
            return *gen;
        }

        DynamoLoader & DynamoManager:: loader()
        {
            if(ld.is_empty())
            {
                ld = new DynamoLoader();
            }
            return *ld;
        }
    }

}


