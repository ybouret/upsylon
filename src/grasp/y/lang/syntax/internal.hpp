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

            //! internal rule base class
            class Internal : public Rule
            {
            public:
                virtual ~Internal() throw(); //!< destructor

            protected:
                explicit Internal(const uint32_t        ,
                                  const string         &,
                                  const std::type_info &); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

        }

    }
}

#endif

