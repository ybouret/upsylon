
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
                typedef memory::magazine<Unit> Supply;  //!< alias
                typedef Supply::auto_ptr       Pointer; //!< alias
                typedef Supply::list_          List;    //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                static Unit *  Create(const Context &,
                                      const Tag     &,
                                      size_t        &);  //!< using supply
                static void    Delete(Unit *) throw();   //!< using supply
                static Supply &Instance();               //!< instance
                
                exception & cat_label(exception &e) const throw(); //!< "<label>"
                
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag     label; //!< creating rule label
                const size_t  stamp; //!< creation stamp
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Unit);
                virtual ~Unit() throw();                                           //!< cleanup
                explicit Unit(const Context &, const Tag &, const size_t) throw(); //!< setup
                friend class memory::magazine<Unit>;
            };

        }
    }

	Y_MAGAZINE_DECL(Jive::Lexical::Unit)


}

#endif
