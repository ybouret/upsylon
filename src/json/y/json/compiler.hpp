//! \file
#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/json/value.hpp"
#include "y/lang/module.hpp"

namespace upsylon
{
    namespace JSON
    {
        class Compiler;

        struct Load
        {
            static void From( Lang::Module *module, Value &value );
        };
    }
}

#endif

