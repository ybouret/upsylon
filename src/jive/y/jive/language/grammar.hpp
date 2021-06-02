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
                typedef Axiom Axiom; //!< alias
                
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
                // terminal
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
                // repeat
                //______________________________________________________________

                //! create a named repeat
                template <typename ID>
                const Repeat & rep(const ID    &id,
                                   const Axiom &axiom,
                                   const size_t atLeast)
                {
                    return add( new Repeat(id,axiom,atLeast,false) );
                }

                //! create an automatic repeat
                const Repeat & rep(const Axiom &axiom,
                                   const size_t atLeast);



                //______________________________________________________________
                //
                // option
                //______________________________________________________________

                //! create a named option
                template <typename ID>
                const Option & opt(const ID &id, const Axiom &axiom)
                {
                    return add( new Option(id,axiom) );
                }

                //! create an automatic option
                const Option & opt(const Axiom &axiom);

                //______________________________________________________________
                //
                // running on source
                //______________________________________________________________

                //! accept the root Axiom
                xNode *run(Source &source, Lexer &lexer);


                
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

