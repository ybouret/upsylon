//! \file
#ifndef Y_CONFIRM_INCLUDED
#define Y_CONFIRM_INCLUDED 1

#include <iostream>
#include "y/os/error.hpp"

//! permanent assert-like
#define Y_CONFIRM(EXPR,WHEN) do {\
if( !(EXPR) ) {\
std::cerr << __FILE__ << ":" << __LINE__ << ": UNCONFIRMED <" << #EXPR << ">" << std::endl;\
upsylon::fatal_error(WHEN);\
}\
} while(false)

#endif

