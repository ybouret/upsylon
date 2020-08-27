
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
            explicit Token() throw();
            virtual ~Token() throw();
            Token(const Token &);
            Token & operator=(const Token &);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            string toString() const;
        };

    }

}

#endif

