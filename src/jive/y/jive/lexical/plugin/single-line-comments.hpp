//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_SINGLE_LINE_COMMENTS_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_SINGLE_LINE_COMMENTS_INCLUDED 1

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
            //! single line comments
            //
            //__________________________________________________________________
            class SingleLineComments : public Plugin
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~SingleLineComments() throw();



            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleLineComments);
            };

        }
    }
}

#endif
