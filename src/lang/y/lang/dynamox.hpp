
#ifndef Y_LANG_DYNAMOX_INCLUDED
#define Y_LANG_DYNAMOX_INCLUDED 1

#include "y/lang/dynamo.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{
    namespace Lang
    {

#define Y_DYNAMOX(NAME)                                  \
#NAME " grammar", NAME##_grammar,sizeof(NAME##_grammar), \
NAME##_terminals, sizeof(NAME##_terminals),              \
NAME##_internals, sizeof(NAME##_internals)

        //! generate a compiler from compiled data from y-dynamoc
        class DynamoX : public Dynamo
        {
        public:
            virtual ~DynamoX() throw();
            explicit DynamoX(const char  *data_name,
                             const void  *data_addr,
                             const size_t data_size,
                             const void  *terminals_data,
                             const size_t terminals_size,
                             const void  *internals_data,
                             const size_t internals_size);

            const hashing::mperf terminalHash;
            const hashing::mperf internalHash;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoX);
        };


    }
}

#endif

