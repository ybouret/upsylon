//! \file
#ifndef Y_LANG_TOKEN_CONVERT_INCLUDED
#define Y_LANG_TOKEN_CONVERT_INCLUDED 1

#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! fast conversion with stringify
        struct TokenConvert
        {
            static unit_t ToUnit( const Token &token, const char *context=NULL); //!< signed   type
            static size_t ToSize( const Token &token, const char *context=NULL); //!< unsigned type
        };
    }
}


#endif
