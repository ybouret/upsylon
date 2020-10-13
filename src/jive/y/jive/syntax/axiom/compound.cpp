

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
                try
                {
                    axiom.called_by(*this);
                }
                catch(...)
                {
                    delete pop_back();
                    throw;
                }
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

#
        }
        
    }
    
}

