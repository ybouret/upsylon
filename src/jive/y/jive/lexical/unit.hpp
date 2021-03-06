
//! \file

#ifndef Y_JIVE_LEXICAL_UNIT_INCLUDED
#define Y_JIVE_LEXICAL_UNIT_INCLUDED 1

#include "y/jive/token.hpp"
#include "y/memory/magazine.hpp"

namespace upsylon {


    namespace Jive {

        namespace Language
        {
            class Grammar;
            class Aggregate;
        }

        namespace Lexical {

            //__________________________________________________________________
            //
            //
            //! a unit is produced by a lexical rule
            //
            //__________________________________________________________________
            class Unit : public Token, public Context, public dnode<Unit>
            {
            public:



                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef memory::magazine<Unit> Supply;  //!< alias
                typedef Supply::auto_ptr       Pointer; //!< alias
                typedef Supply::list_          List;    //!< alias

                //! usage or this unit during parsing
                enum Usage
                {
                    Core, //!< in an aggregate
                    Done  //!< end of an aggregate
                };


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                static Unit *  Create(const Context &,
                                      const Tag     &,
                                      size_t        &);  //!< using supply
                static void    Delete(Unit *) throw();   //!< using supply
                static Supply &Instance();               //!< instance
                exception     &labelTo(exception &e)                              const throw();                      //!< "<label>"
                exception     &writeTo(exception &e, const bool withText)         const throw(); //!< "<label> = '...'"
                exception     &writeOn(exception &e, const Language::Grammar &)   const throw(); //!< using Terminal::Type
                const char    *usageText()                                        const throw(); //!< core|done

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag                  label; //!< creating rule label
                const size_t               stamp; //!< creation stamp
                const Language::Aggregate *owner; //!< apparent aggregate owner
                Usage                      usage; //!< usage to guess errors

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
