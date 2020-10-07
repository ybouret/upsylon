

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
            
            //__________________________________________________________________
            //
            //
            //! Compound base class for a list of Axioms
            //
            //__________________________________________________________________
            class Compound : public Internal, public Axiom::Reference::List
            {
            public:
                //! cleanup
                virtual ~Compound() throw();
                
                //! helper to grow axioms list
                Compound & operator<<(const Axiom &);
                
                
            protected:
                //! setup
                template <typename ID>
                explicit Compound(const ID      &i,
                                  const uint32_t t) :
                Internal(i,t)
                {
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };
            
            
        }
        
    }
    
}

#endif
