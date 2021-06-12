#include "y/jive/language/axiom.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            bool Axiom:: Verbose = false;
            bool Axiom:: Tracing = false;

            const char  Axiom:: Accepted[] = "accepted";
            const char  Axiom:: Rejected[] = "rejected";


            string   Axiom:: gvName() const
            {
                return string_convert::to_printable(*name);
            }

            Axiom:: ~Axiom() throw()
            {
            }

            ios::indent Observer::indent() const throw()
            {
                return ios::indent(depth);
            }

            const char * Axiom:: Status(const bool flag) throw()
            {
                return flag ? Accepted : Rejected;
            }
            
            

        }
    }

}


