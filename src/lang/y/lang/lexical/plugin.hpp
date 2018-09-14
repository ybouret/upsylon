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
            class Translator; //!< forward declaration

            //! plugin API
            class Plugin : public Scanner
            {
            public:
                typedef intr_ptr<string,Plugin> Pointer; //!< alias for database
                
                virtual ~Plugin() throw(); //!< desctructor

                const string trigger;                 //!< triggering expression
                virtual void Init(const Token &) = 0; //!< what do do when plugin is called

            protected:
                explicit Plugin(Translator &attached, const string &id, const char   *rx); //!< initialize
                explicit Plugin(Translator &attacher, const string &id, const string &rx); //!< initialize

                Translator &lexer;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }
    }
}

#endif
