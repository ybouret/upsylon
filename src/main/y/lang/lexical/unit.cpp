#include "y/lang/lexical/unit.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        Lexeme:: ~Lexeme() throw()
        {
        }

        Lexeme:: Lexeme(const Tag &ruleLabel) :
        object(),
        Token(),Base(),label(ruleLabel)
        {
        }

        Lexeme:: Lexeme(const Lexeme &other):
        object(),
        Token(other), Base(), label(other.label)
        {}

        const CharInfo & Lexeme:: info() const
        {
            const Char *ch = head;
            if(!ch) throw exception("Lexeme::info(Empty Lexeme)");
            return *ch;
        }


        int Lexeme:: line() const throw()
        {
            return (head) ? head->line : 0;
        }

        int Lexeme:: column() const throw()
        {
            return (head) ? head->column : 0;
        }

        const char * Lexeme:: origin() const throw()
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
