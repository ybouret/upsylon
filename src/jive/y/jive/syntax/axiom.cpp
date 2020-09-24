
#include "y/jive/syntax/axiom.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Axiom:: ~Axiom() throw()
            {
                aliasing::_(uuid) = 0;
                aliasing::_(self) = 0;
            }

            Axiom:: Axiom(const uint32_t u, const string *s) throw() :
            uuid(u),
            name(s),
            self(0)
            {
            }

            void Axiom:: I_am(const void *addr) throw()
            {
                assert(0==self);
                assert(0!=addr);
                aliasing::_(self) = addr;
            }

            bool Axiom:: isTerminal() const throw()
            {
                return TermUUID == uuid;
            }

            bool Axiom:: isInternal() const throw()
            {
                return TermUUID != uuid;
            }



        }
    }
}

