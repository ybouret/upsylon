

//! \file

#ifndef Y_JARGON_TOKEN_CONVERT_INCLUDED
#define Y_JARGON_TOKEN_CONVERT_INCLUDED 1

#include "y/jargon/token.hpp"

namespace upsylon {
    
    
    namespace Jargon {

        //! conversion of tokens
        struct TokenConvert
        {
            static void To(string &target, const Token &token); //!< to string
            static void To(double &target, const Token &token); //!< to double
            
        };
    }
    
}

#endif
