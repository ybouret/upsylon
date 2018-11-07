//! \file
#ifndef Y_SEEM_COMPILER_INCLUDED
#define Y_SEEM_COMPILER_INCLUDED 1

#include "y/lang/dynamox.hpp"

namespace upsylon
{
    namespace SEEM
    {
        class Compiler : public Lang::DynamoX
        {
        public:
            explicit Compiler(const bool verbose=false);         //!< setup
            virtual ~Compiler() throw(); //!< desctructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };
    }
}


#endif

