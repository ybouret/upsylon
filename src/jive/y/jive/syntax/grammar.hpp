
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
         
            class Grammar
            {
            public:
                typedef Axiom                    *AxiomPointer;
                typedef suffix_tree<AxiomPointer> AxiomRegistry;
                
                const Tag name;
                
                virtual ~Grammar() throw();
                
                template <typename ID> inline
                explicit Grammar( const ID &id ) :
                name(id)
                {
                }
                

                //! append axiom to list and registry
                void add(Axiom *axiom);
                
                template <typename ID> inline
                const Axiom *query( const ID &id ) const throw()
                {
                    const AxiomPointer *ppA = registry.search_by(id);
                    if(ppA)
                    {
                        return *ppA;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Axiom::List   axioms;
                AxiomRegistry registry;
            };
            
        }
        
    }
    
}

#endif
