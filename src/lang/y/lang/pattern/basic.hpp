//! \file
#ifndef Y_LANG_PATTERN_BASIC_INCLUDED
#define Y_LANG_PATTERN_BASIC_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon
{
    namespace Lang
    {

        class Any1 : public Pattern
        {
        public:
            static const uint32_t UUID = Y_FOURCC('A','N','Y','1');
        private:
            Y_DISABLE_ASSIGN(Any1);
        };

    }
}

#endif

