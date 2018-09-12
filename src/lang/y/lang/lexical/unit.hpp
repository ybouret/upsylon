//! \file
#ifndef Y_LANG_LEXICAL_UNIT_INCLUDED
#define Y_LANG_LEXICAL_UNIT_INCLUDED 1

#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! a Lexeme
        class Lexeme : public Token
        {
        public:
            Lexeme        *next;  //!< for list
            Lexeme        *prev;  //!< for list
            const Origin label; //!< rule label

            //! initialize
            explicit Lexeme(const Origin &ruleLabel);
            //! copy content and label
            Lexeme(const Lexeme &other);
            //! destructor
            virtual ~Lexeme() throw();

            //! list/cache
            typedef core::list_of_cpp<Lexeme> List;

        private:
            Y_DISABLE_ASSIGN(Lexeme);

        };


    }
}

#endif

