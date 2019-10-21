
#include "y/associative/b-tree.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        void btree:: throw_on_insert_failure()
        {
            throw exception("btree: unexpected copy insert failure!!!");
        }

    }
}

