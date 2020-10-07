

//! \file
#ifndef Y_JIVE_SYNTAX_COMPOUND_INCLUDED
#define Y_JIVE_SYNTAX_COMPOUND_INCLUDED 1

#include "y/jive/syntax/axiom/internal.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            typedef Axiom::Reference::List Manifest; //!< alias

            //__________________________________________________________________
            //
            //
            //! Compound base class for a list of Axioms
            //
            //__________________________________________________________________
            class Compound : public Internal, public Manifest
            {
            public:
                //! cleanup
                virtual ~Compound() throw();
                
                //! helper to grow axioms list
                Compound & operator<<(const Axiom &);
                
                Y_JIVE_AXIOM_ATTACH_DECL();

            protected:
                //! setup
                template <typename ID>
                explicit Compound(const ID      &i,
                                  const uint32_t t) :
                Internal(i,t),
                Manifest()
                {
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };
            
            
        }
        
    }
    
}

#endif
