
#include "y/jive/language/axiom/compound.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Compound::~Compound() throw()
            {
            }

            Compound & Compound:: operator<<(const Axiom &axiom)
            {
                push_back( new Axiom::Reference(axiom) );
                return *this;
            }


        }

    }

}
