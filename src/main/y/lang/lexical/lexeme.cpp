#include "y/lang/lexical/lexeme.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Lang {

        namespace Lexical {

            Unit:: ~Unit() throw()
            {
            }

            Unit:: Unit(const Tag &ruleLabel) throw():
            object(),
            Token(),Base(),label(ruleLabel)
            {
            }

            Unit:: Unit(const Unit &other):
            object(),
            Token(other), Base(), label(other.label)
            {}

            const CharInfo & Unit:: info() const
            {
                const Char *ch = head;
                if(!ch) throw exception("Lexeme::info: no chars!");
                return *ch;
            }


            int Unit:: line() const throw()
            {
                return (head) ? head->line : 0;
            }

            int Unit:: column() const throw()
            {
                return (head) ? head->column : 0;
            }

            const char * Unit:: origin() const throw()
            {
                if(head)
                {
                    return **(head->origin);
                }
                else
                {
                    return "?";
                }
            }

        }
    }
}
