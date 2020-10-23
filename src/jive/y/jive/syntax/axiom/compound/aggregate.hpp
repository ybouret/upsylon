


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
            /**
             - must accept all members
             - may accept empty tree!
             */
            //
            //__________________________________________________________________
            class Aggregate : public Compound
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('A','G','G',' '); //!< UUID

                //! behaviors
                enum Type
                {
                    Standard,  //!< alway produce a named aggregate
                    Variadic,  //!< merge if only one child, keep multiple
                    Grouping   //!< transient grouping
                };

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                template <typename ID> inline
                explicit Aggregate(const ID &i, const Type t) :
                Compound(i,UUID),
                type(t)
                {
                    Y_JIVE_AXIOM(Aggregate);
                }

                //! cleanup
                virtual ~Aggregate() throw();
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                bool       named() const throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Type type; //!< the behavior

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };
            
            
        }
        
    }
    
}

#endif
