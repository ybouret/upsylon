
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_STRINGS_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_STRINGS_INCLUDED 1

#include "y/jive/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            class String : public Plugin
            {
            public:
                const char delimiter;

                virtual ~String() throw();

                template <typename ID>
                explicit String(const ID &id, const char C, Queue &q) :
                Plugin(id,C,q),
                delimiter(C),
                s()
                {
                    setup();
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(String);
                Token s;
                void  setup();
                virtual void OnInit(const Token &);
                void         OnQuit(const Token &);
                void         OnChar(const Token &);
            };

        }

    }

}

#endif

