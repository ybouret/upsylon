#include "y/lang/char.hpp"

namespace upsylon
{
    namespace Lang
    {
        Object::  Object() throw() {}
        Object:: ~Object() throw() {}

        Manager:: Manager() throw() : singleton<Manager>(), Char::Pool()
        {
        }

        Manager:: ~Manager() throw()
        {
        }


    }

}

