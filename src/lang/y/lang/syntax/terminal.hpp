
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
                //! class  identfier
                static const uint32_t UUID = Y_FOURCC('T', 'E', 'R', 'M');
                
                explicit Terminal( const string &id ); //!< initialize
                virtual ~Terminal() throw();           //!< destructor

                //! accept if there is a lexeme with label==name
                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };

        }

    }

}

#endif

