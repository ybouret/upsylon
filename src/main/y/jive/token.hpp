
//! \file

#ifndef Y_JIVE_TOKEN_INCLUDED
#define Y_JIVE_TOKEN_INCLUDED 1

#include "y/jive/char.hpp"

namespace upsylon {

    class exception;

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! a token is am evolved Char::List
        //
        //______________________________________________________________________
        class Token : public CountedObject, public Char::List
        {
        public:
            typedef arc_ptr<Token> Handle;
            
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
            string toString()       const; //!< codes->chars
            string toVisible()      const; //!< codes->visible
            string toPrintable()    const; //!< codes->printable
            exception & cat(exception &) const throw(); //!< append to exception
        };

    }

}

#endif

