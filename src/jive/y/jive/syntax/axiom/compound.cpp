

#include "y/jive/syntax/axiom/compound.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Compound:: ~Compound() throw()
            {
            }
            
            Compound & Compound:: operator<<(const Axiom &axiom)
            {
                push_back( new Axiom::Reference(axiom) );
                return *this;
            }

            void Compound:: attach(Axiom::Registry &db) const
            {
                if(new_in(db))
                {
                    for(const Axiom::Reference *ref=head;ref;ref=ref->next)
                    {
                        (**ref).attach(db);
                    }
                }
            }

#if 0
            void Compound:: expect(TermCheckList &ex, Axiom::Registry &db) const
            {
                if(new_in(db))
                {
                    for(const Axiom::Reference *ref=head;ref;ref=ref->next)
                    {
                        (**ref).expect(ex,db);
                    }
                }
            }
#endif
        }
        
    }
    
}

