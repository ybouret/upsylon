#include "y/jive/language/axiom.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            Axiom:: TermLedger:: TermLedger(const Compound &owner) :
            Object(), TermLedger_(),
            next(0),
            from(owner)
            {
            }
            
            Axiom:: TermLedger:: ~TermLedger() throw()
            {
            }

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
            }

            

            const char * Axiom:: Status(const bool flag) throw()
            {
                return flag ? Accepted : Rejected;
            }
            
            

        }
    }

}


