//! \file

#ifndef Y_JIVE_PATTERN_FIRST_CHARS_INCLUDED
#define Y_JIVE_PATTERN_FIRST_CHARS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    namespace Jive
    {
        class FirstChars
        {
        public:
            explicit FirstChars() throw();
            virtual ~FirstChars() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FirstChars);
        };
    }

}

#endif
