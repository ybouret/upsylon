#include "y/lang/char.hpp"

namespace upsylon
{
    namespace Lang
    {
        Char::  Char(const uint8_t c, const CharInfo &info) throw() : object(), CharInfo(info), Base(), code(c) {}
        Char:: ~Char() throw() {}
        Char::  Char(const Char &other) throw() : object(), CharInfo(other), Base(), code(other.code) {}

    }

}


