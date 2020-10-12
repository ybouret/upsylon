
#include "y/jive/syntax/axiom/wildcard.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Wildcard:: ~Wildcard() throw()
            {
            }

            void Wildcard:: attach(Axiom::Registry &db) const
            {
                if( new_in(db) )
                {
                    axiom.attach(db);
                }
            }

            void Wildcard:: expect(TermCheckList &ex, Axiom::Registry &db) const
            {
                if( new_in(db) )
                {
                    axiom.expect(ex,db);
                }
            }

        }

    }

}

