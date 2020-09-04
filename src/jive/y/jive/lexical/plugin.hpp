
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_INCLUDED 1

#include "y/jive/lexical/scanner.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //! specialized, loadable scanner
            //
            //__________________________________________________________________
            class Plugin : public Scanner
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Plugin() throw();

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };

        }
    }
}

#endif
