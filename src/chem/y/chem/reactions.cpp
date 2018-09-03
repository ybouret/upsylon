#include "y/chem/reactions.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Reactions:: ~Reactions() throw()
        {
        }

        Reactions:: Reactions(const size_t n) : ReactionsType(n,as_capacity)
        {
        }

        Reaction & Reactions:: enroll( Reaction *rxn )
        {
            assert(rxn);
            Reaction::Pointer p(rxn);
            if( !insert(p) )
            {
                throw exception("multiple reaction '%s'",*(p->name));
            }
            return *p;
        }


    }

}
