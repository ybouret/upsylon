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
                typedef intr_ptr<string,Plugin> Pointer;
                
                virtual ~Plugin() throw();

                const string trigger;
                virtual void Init(const Token &) = 0;

            protected:
                explicit Plugin(const string &id, const char   *rx);
                explicit Plugin(const string &id, const string &rx);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }
    }
}

#endif
