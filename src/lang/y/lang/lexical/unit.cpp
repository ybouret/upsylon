
#include "y/lang/lexical/unit.hpp"

namespace upsylon
{
    namespace Lang
    {
        Lexeme:: ~Lexeme() throw()
        {
        }

        Lexeme:: Lexeme(const Origin &ruleLabel) :
        Token(), next(0), prev(0), label(ruleLabel)
        {
        }

        Lexeme:: Lexeme(const Lexeme &other):
        Token(other), next(0), prev(0), label(other.label)
        {}

    }
}
