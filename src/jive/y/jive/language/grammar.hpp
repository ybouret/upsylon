//! \file

#ifndef Y_LANG_GRAMMAR_INCLUDED
#define Y_LANG_GRAMMAR_INCLUDED 1

#include "y/jive/language/axiom/all.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            //__________________________________________________________________
            //
            //
            //! Grammar is a list of axioms
            //
            //__________________________________________________________________
            class Grammar
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename ID> inline
                explicit Grammar(const ID &id) :
                name( Tags::Make(id) ), axioms(), registry(), aligned(0)
                {}

                //! cleanup
                virtual ~Grammar() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! axiom is pushed at the back of the axioms and registerd
                void declare(Axiom *axiom);

                //! helper
                template <typename AXIOM> inline
                AXIOM & add(AXIOM *axiom)
                {
                    assert(axiom);
                    declare(axiom);
                    return *axiom;
                }

                //! set Root axiom
                void setRoot(const Axiom &axiom);

                //! get Root axiom
                const Axiom * getRoot() const throw();

                //! query by name
                template <typename ID> inline
                const Axiom * query(const ID &id) const throw()
                {
                    const Axiom * const *ptr = registry.search(id);
                    return ptr ? *ptr : NULL;
                }

                //______________________________________________________________
                //
                // creating Axioms
                //______________________________________________________________

                //! a new terminal
                template <typename ID>
                const Terminal & term(const ID            &i,
                                      const Terminal::Type t)
                {
                    return add( new Terminal(i,t) );
                }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag             name;      //!< shared name
                const Axiom::List     axioms;    //!< axioms
                const Axiom::Registry registry;  //!< registry
                const size_t          aligned;   //!< max of axioms name length

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            };
        }
    }
}

#endif

