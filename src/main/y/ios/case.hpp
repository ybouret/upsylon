//! \file

#ifndef Y_IOS_CASE_INCLUDED
#define Y_IOS_CASE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! helper to choose cases in ouput
    enum CharCase
    {
        Lowercase, //!< lowecase
        Uppercase  //!< uppercase
    };
    
    //! get textual info on case
    const char *CharCaseInfo(const CharCase) throw();
}

#endif

