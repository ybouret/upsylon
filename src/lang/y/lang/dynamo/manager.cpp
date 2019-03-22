
#include "y/lang/dynamo/manager.hpp"


namespace upsylon
{
    namespace Lang
    {
        DynamoManager:: ~DynamoManager() throw()
        {
        }

        DynamoManager:: DynamoManager() throw() :
        ld(0)
        {
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


