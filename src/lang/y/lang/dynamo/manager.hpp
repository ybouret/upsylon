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
            DynamoLoader & loader();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoManager);
            friend class singleton<DynamoManager>;
            explicit DynamoManager() throw();
            virtual ~DynamoManager() throw();
            static const at_exit::longevity life_time = -13;

            auto_ptr<DynamoLoader> ld;

        };

    }
}

#endif

