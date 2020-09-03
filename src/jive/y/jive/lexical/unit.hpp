
//! \file

#ifndef Y_JIVE_LEXICAL_UNIT_INCLUDED
#define Y_JIVE_LEXICAL_UNIT_INCLUDED 1

#include "y/jive/token.hpp"

namespace upsylon {


    namespace Jive {

        namespace Lexical {

            //! a unit is created by a lexical rule
            class Unit : public Token, public Context, public inode<Unit>
            {
            public:
                typedef core::list_of_cpp<Unit> List; //!< alias for Lexemes

                virtual ~Unit() throw(); //!< cleanup

                //! setup
                explicit Unit(const Context &,
                              const Tag     &) throw();


                const Tag label;                         //!< creating rule label
                
                
            private:
                Y_DISABLE_ASSIGN(Unit);
            };

        }
    }

}

#endif
