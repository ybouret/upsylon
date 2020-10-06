


//! \file
#ifndef Y_JIVE_SYNTAX_AGG_INCLUDED
#define Y_JIVE_SYNTAX_AGG_INCLUDED 1

#include "y/jive/syntax/axiom/compound.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            
            //__________________________________________________________________
            //
            //
            //! aggregate axioms
            //
            //__________________________________________________________________
            class Aggregate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A','G','G',' '); //!< UUID
                
                //! cleanup
                virtual ~Aggregate() throw();
                
                //! setup
                template <typename ID> inline
                explicit Aggregate(const ID &i) :
                Compound(i,UUID)
                {
                    Y_JIVE_AXIOM(Aggregate);
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };
            
            
        }
        
    }
    
}

#endif
