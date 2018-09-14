//! \file
#ifndef Y_LANG_LEXICAL_JSTRING_INCLUDED
#define Y_LANG_LEXICAL_JSTRING_INCLUDED 1

#include "y/lang/lexical/plugin.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            class jString : public Plugin
            {
            public:
                explicit jString(const string &id);
                virtual ~jString() throw();
                virtual void Init(const Token &);

                Token content;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);
            };

        }

    }

}

#endif

