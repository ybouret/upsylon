
//!\file
#ifndef Y_LANG_SYNTAX_TERMINAL_INCLUDED
#define Y_LANG_SYNTAX_TERMINAL_INCLUDED

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! accept a lexeme with same label
            class Terminal : public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('T', 'E', 'R', 'M');

                const string label;
                
                explicit Terminal( const string &id );
                virtual ~Terminal() throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}

#endif

