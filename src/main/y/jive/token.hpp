
//! \file

#ifndef Y_JIVE_TOKEN_INCLUDED
#define Y_JIVE_TOKEN_INCLUDED 1

#include "y/jive/char.hpp"

namespace upsylon {

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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Token() throw(); //!< create empty
            virtual ~Token() throw(); //!< cleanup
            Token(const Token &);     //!< copy
            Token & operator=(const Token &); //!< assign by copy/swap
            

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            string toString() const; //!< codes->chars
        };

    }

}

#endif

