
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_MULTI_LINES_COMMENTS_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_MULTI_LINES_COMMENTS_INCLUDED 1

#include "y/jive/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //! multiple lines comments
            //
            //__________________________________________________________________
            class MultiLinesComments : public Plugin
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~MultiLinesComments() throw();



            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiLinesComments);
            };

        }
    }
}

#endif
