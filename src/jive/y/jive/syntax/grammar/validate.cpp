
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            void  Grammar:: validate() const
            {
                const size_t    na = axioms.size;
                if(na<=0)
                {
                    
                }
                Axiom::Registry db;
                for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                {
                    axiom->attach(db);
                }
            }

        }

    }
}

