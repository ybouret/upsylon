//! \file
#ifndef Y_LANG_LEXICAL_UNIT_INCLUDED
#define Y_LANG_LEXICAL_UNIT_INCLUDED 1

#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! a Lexeme is a labeled Token
        class Lexeme : public Token
        {
        public:
            typedef core::list_of_cpp<Lexeme> List; //!< alias

            Lexeme        *next;  //!< for list
            Lexeme        *prev;  //!< for list
            const Origin   label; //!< rule label


            explicit Lexeme(const Origin &ruleLabel);//!< initialize
            Lexeme(const Lexeme &other);             //!<copy content and label
            virtual ~Lexeme() throw();               //! destructor
            const CharInfo & info() const;           //!< info for first token
            int              line() const throw();   //!< 0 if empty
            int              column() const throw(); //!< 0 if empty
        private:
            Y_DISABLE_ASSIGN(Lexeme);

        };
        


    }
}

#endif

