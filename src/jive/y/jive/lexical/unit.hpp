
//! \file

#ifndef Y_JIVE_LEXICAL_UNIT_INCLUDED
#define Y_JIVE_LEXICAL_UNIT_INCLUDED 1

#include "y/jive/token.hpp"
#include "y/memory/magazine.hpp"

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
                typedef memory::magazine<Unit> Supply;
                typedef Supply::auto_ptr       Pointer;
                typedef Supply::list_          List;

                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static Unit *Create(const Context &, const Tag &); //!< using supply
                static void  Delete(Unit *) throw();               //!< using supply
                

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag label; //!< creating rule label
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Unit);
                virtual ~Unit() throw();                             //!< cleanup
                explicit Unit(const Context &, const Tag &) throw(); //!< setup
                friend class memory::magazine<Unit>;
                
            };

        }
    }

}

#endif
