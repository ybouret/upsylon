//! \file

#ifndef Y_DYNAMO_MANAGER_INCLUDED
#define Y_DYNAMO_MANAGER_INCLUDED 1

#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/loader.hpp"


namespace upsylon
{
    namespace Lang
    {

        class DynamoManager : public singleton<DynamoManager>
        {
        public:
            DynamoGenerator & generator(); //!< generator management
            DynamoLoader    & loader();    //!< loader management

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoManager);
            friend class singleton<DynamoManager>;
            explicit DynamoManager() throw();
            virtual ~DynamoManager() throw();
            static const at_exit::longevity life_time = -13;

            auto_ptr<DynamoGenerator> gen;
            auto_ptr<DynamoLoader>    ld;

        };

    }
}

#endif

