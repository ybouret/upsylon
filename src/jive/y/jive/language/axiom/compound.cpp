
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

            string Compound:: enumerate(const char sep) const
            {
                string ans = '{';
                if(size>0)
                {
                    const Reference *ref = head;
                    ans += *( (**ref).name );
                    while(NULL!=(ref=ref->next))
                    {
                        ans += sep;
                        ans += *( (**ref).name );
                    }
                }
                else
                {
                    ans += core::ptr::nil;
                }
                ans += '}';
                return ans;
            }


        }

    }

}
