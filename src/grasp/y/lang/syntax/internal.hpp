
//! \file
#ifndef Y_LANG_SYNTAX_INTERNAL_INCLUDED
#define Y_LANG_SYNTAX_INTERNAL_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Internal : public Rule
            {
            public:
                virtual ~Internal() throw();

            protected:
                explicit Internal(const uint32_t i, const string &n);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

        }

    }
}

#endif

