//! \file
#ifndef Y_IOS_GRAPHVIZ_INCLUDED
#define Y_IOS_GRAPHVIZ_INCLUDED 1

#include "y/strfwd.hpp"

namespace upsylon
{
    namespace ios
    {
        //! GrapViz calls
        struct GraphViz
        {
            //! render a filename into its png counterpart
            static bool Render( const string &filename, bool keepFile = false );
        };
    }
}

#endif

