//! \file

#ifndef Y_JIVE_TOKEN_INCLUDED
#define Y_JIVE_TOKEN_INCLUDED 1

#include "y/jive/char.hpp"

namespace upsylon
{

    class exception; //!< forward declaration

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! a token is an evolved Char::List
        //
        //______________________________________________________________________
        class Token : public CountedObject, public Char::List
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef arc_ptr<Token> Handle; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Token() throw();         //!< create empty
            virtual ~Token() throw();         //!< cleanup
            Token(const Token &);             //!< copy
            Token & operator=(const Token &); //!< assign by copy/swap


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            string       toString(size_t nskip=0,
                                  size_t ntrim=0) const; //!< codes->chars
            string       toVisible()       const; //!< codes->visible
            string       toPrintable()     const; //!< codes->printable
            string       toEncoded()       const; //!< codes->encoded
            Token     &  skip(size_t n)  throw(); //!< skip n chars
            Token     &  trim(size_t n)  throw(); //!< skip n chars
            Token     &  chop(size_t n)  throw(); //!< trim(n).skip(n)

            Token & operator<<(Char *ch);      //!< push_back(ch)
            Token & operator<<(const Token &); //!< push_back a copy

            exception &  charsTo(exception &) const throw(); //!< append to exception
            
        private:
            string translate(const char * table[]) const;
        };

    }

}

#endif

