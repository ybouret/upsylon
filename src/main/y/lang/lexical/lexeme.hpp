//! \file
#ifndef Y_LANG_LEXICAL_UNIT_INCLUDED
#define Y_LANG_LEXICAL_UNIT_INCLUDED 1

#include "y/lang/token.hpp"

namespace upsylon {

    namespace Lang {

        namespace Lexical {

            //! a Unit is a labeled Token
            class Unit : public Token, public core::inode<Unit>
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                typedef core::inode<Unit>       Base; //!< alias
                typedef core::list_of_cpp<Unit> List; //!< alias

                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                explicit Unit(const Tag &) throw();   //!< initialize
                Unit(const Unit &other);             //!< copy content and label
                virtual ~Unit() throw();               //!< destructor

                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
                const CharInfo & info()   const;         //!< info for first token
                int              line()   const throw(); //!< 0 if empty
                int              column() const throw(); //!< 0 if empty
                const char     * origin() const throw(); //!< try to guess origin

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Tag      label; //!< rule label

            private:
                Y_DISABLE_ASSIGN(Unit);

            };

        }

        typedef Lexical::Unit Lexeme; //!< alias
    }
}

#endif

