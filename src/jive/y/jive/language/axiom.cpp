#include "y/jive/language/axiom.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            bool Axiom:: Verbose = false;
            
            const char  Axiom:: Accepted[] = "accepted";
            const char  Axiom:: Rejected[] = "rejected";


            string   Axiom:: gvName() const
            {
                return string_convert::to_printable(*name);
            }

            Axiom:: ~Axiom() throw()
            {
                aliasing::_(uuid) = 0;
                while(host)
                {
                    Host *next = host->next;
                    delete host;
                    host = next;
                }
            }

        

            const char * Axiom:: Status(const bool flag) throw()
            {
                return flag ? Accepted : Rejected;
            }
            
            

        }
    }

}


