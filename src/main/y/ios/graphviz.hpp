//! \file
#ifndef Y_IOS_GRAPHVIZ_INCLUDED
#define Y_IOS_GRAPHVIZ_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    namespace ios
    {
        struct GraphViz
        {
            static bool Render( const string &filename, bool keepFile = false );
        };
    }
}

#endif

