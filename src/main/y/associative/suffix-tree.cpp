#include "y/associative/suffix-tree.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        suffix_tree:: suffix_tree() throw()
        {
        }

        suffix_tree:: ~suffix_tree() throw()
        {
        }

        void suffix_tree:: throw_on_insert_failure( const path &key) 
        {
            throw exception("unexpected suffix_tree insert failure: key.size=%u", unsigned(key.size()));
        }

    }
}

