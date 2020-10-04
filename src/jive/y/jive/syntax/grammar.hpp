
//! \file
#ifndef Y_JIVE_SYNTAX_GRAMMAR_INCLUDED
#define Y_JIVE_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/jive/syntax/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //! Grammar
            //
            //__________________________________________________________________
            class Grammar
            {
            public:
                //______________________________________________________________
                //
                // type and definitions
                //______________________________________________________________
                typedef Axiom                    *AxiomPointer;  //!< alias
                typedef suffix_tree<AxiomPointer> AxiomRegistry; //!< aias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! cleanup
                virtual ~Grammar() throw();
                
                //! setup
                template <typename ID> inline
                explicit Grammar( const ID &id ) :
                name( Tags::Make(id) ),
                axioms(), registry()
                {
                }
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! append axiom to list and registry
                void declare(Axiom *axiom);

                //! append axiom
                template <typename AXIOM> inline
                AXIOM & add( AXIOM *axiom )
                {
                    declare(axiom);
                    return *axiom;
                }

                //! check existence
                template <typename ID> inline
                const Axiom *query( const ID &id ) const throw()
                {
                    const AxiomPointer *_ = registry.search_by(id);
                    return _ ? *_ : NULL;
                }
                
                //! get the root axiom
                const Axiom *getRoot() const throw();
                
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag name; //!< shared name

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Axiom::List   axioms;
                AxiomRegistry registry;
            };
            
        }
        
    }
    
}

#endif
