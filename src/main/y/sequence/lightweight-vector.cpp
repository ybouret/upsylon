#include "y/sequence/lightweight-vector.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        void lw_vector::  throw_on_memory_ops()
        {
            throw exception("no memory operations for lightweight-vector!");
        }

    }
}
