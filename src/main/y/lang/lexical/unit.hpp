//! \file
#ifndef Y_LANG_LEXICAL_UNIT_INCLUDED
#define Y_LANG_LEXICAL_UNIT_INCLUDED 1

#include "y/lang/token.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! a Lexeme is a labeled Token
        class Lexeme : public Token, public core::inode<Lexeme>
        {
        public:
            typedef core::inode<Lexeme>       Base; //!< alias
            typedef core::list_of_cpp<Lexeme> List; //!< alias

            const Tag      label; //!< rule label


            explicit Lexeme(const Tag &ruleLabel);   //!< initialize
            Lexeme(const Lexeme &other);             //!< copy content and label
            virtual ~Lexeme() throw();               //!< destructor
            const CharInfo & info() const;           //!< info for first token
            int              line() const throw();   //!< 0 if empty
            int              column() const throw(); //!< 0 if empty
            const char      *origin() const throw(); //!< try to guess origin
            
        private:
            Y_DISABLE_ASSIGN(Lexeme);

        };
        


    }
}

#endif

