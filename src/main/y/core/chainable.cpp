
#include "y/core/chainable.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        chainable::  chainable() throw() {}

        chainable:: ~chainable() throw() {}

        void chainable:: exceeds_capacity() const
        {
            throw exception("chainable.put_all(exceeds capacity!)");
        }

    }
}
