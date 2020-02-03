//! \file
#ifndef Y_DYNAMO_TYPES_INCLUDED
#define Y_DYNAMO_TYPES_INCLUDED 1

#include "y/lang/types.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! base class for objects in Dynamo
        class DynamoObject : public Object
        {
        public:
            explicit DynamoObject() throw(); //!< constructor
            virtual ~DynamoObject() throw(); //!< destructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoObject);
        };
        
    }
}
#endif

