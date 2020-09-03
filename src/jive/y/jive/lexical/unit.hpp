
//! \file

#ifndef Y_JIVE_LEXICAL_UNIT_INCLUDED
#define Y_JIVE_LEXICAL_UNIT_INCLUDED 1

#include "y/jive/token.hpp"

namespace upsylon {


    namespace Jive {

        namespace Lexical {

            //__________________________________________________________________
            //
            //
            //! a unit is produced by a lexical rule
            //
            //__________________________________________________________________
            class Unit : public Token, public Context, public inode<Unit>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<Unit> List; //!< alias for Lexemes

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Unit() throw();                             //!< cleanup
                explicit Unit(const Context &, const Tag &) throw(); //!< setup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag label; //!< creating rule label
                
                
            private:
                Y_DISABLE_ASSIGN(Unit);
            };

        }
    }

}

#endif
