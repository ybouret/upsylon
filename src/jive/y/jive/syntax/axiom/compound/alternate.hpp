



//! \file
#ifndef Y_JIVE_SYNTAX_ALT_INCLUDED
#define Y_JIVE_SYNTAX_ALT_INCLUDED 1

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
            /**
             - accept one of the member
             - try again if the accepted member returns an empty tree
             - so, may return empty
             */
            //
            //__________________________________________________________________
            class Alternate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A','L','T',' '); //!< UUID
                
                //! cleanup
                virtual ~Alternate() throw();
                
                //! setup
                template <typename ID> inline
                explicit Alternate(const ID &i) :
                Compound(i,UUID)
                {
                    Y_JIVE_AXIOM(Alternate);
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };
            
            
        }
        
    }
    
}

#endif
