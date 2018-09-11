//! \file
#ifndef Y_LANG_LEXICAL_UNIT_INCLUDED
#define Y_LANG_LEXICAL_UNIT_INCLUDED 1

#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            //! a Lexeme
            class Unit : public Token
            {
            public:
                Unit        *next;  //!< for list
                Unit        *prev;  //!< for list
                const Origin label; //!< rule label

                //! initialize
                explicit Unit(const Origin &ruleLabel);
                //! copy content and label
                Unit(const Unit &other);
                //! destructor
                virtual ~Unit() throw();

                //! list/cache
                typedef core::list_of_cpp<Unit> List;

            private:
                Y_DISABLE_ASSIGN(Unit);

            };

        }
    }
}

#endif

