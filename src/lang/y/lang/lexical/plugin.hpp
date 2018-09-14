//! \file
#ifndef Y_LANG_LEXICAL_PLUGIN_INCLUDED
#define Y_LANG_LEXICAL_PLUGIN_INCLUDED 1

#include "y/lang/lexical/scanner.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            class Plugin : public Scanner
            {
            public:
                virtual ~Plugin() throw();
                
            protected:
                explicit Plugin(const string &id);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }
    }
}

#endif
