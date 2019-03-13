//! \file
#ifndef Y_LANG_DYNAMOX_INCLUDED
#define Y_LANG_DYNAMOX_INCLUDED 1

#include "y/lang/dynamo.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! use results of y-dynamo to format a DynamoX compiler
#define Y_DYNAMOX(NAME)                                  \
#NAME " grammar", NAME##_grammar,sizeof(NAME##_grammar), \
NAME##_terminals, sizeof(NAME##_terminals),              \
NAME##_internals, sizeof(NAME##_internals)

        //! generate a compiler from compiled data from y-dynamo
        class DynamoX : public Dynamo
        {
        public:
            virtual ~DynamoX() throw(); //!< destructor

            //! initialize
            explicit DynamoX(const char  *data_name,
                             const void  *data_addr,
                             const size_t data_size,
                             const void  *terminals_data,
                             const size_t terminals_size,
                             const void  *internals_data,
                             const size_t internals_size,
                             const bool   verbose=false);

            const hashing::mperf terminalHash; //!< hash table for terminals
            const hashing::mperf internalHash; //!< hash table for internals

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoX);
        };


    }
}

#endif

