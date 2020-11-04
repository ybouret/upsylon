
#include "y/associative/hash/meta-node.hpp"
#include "y/object.hpp"

namespace upsylon
{

    hash_meta_node * hash_meta_node:: acquire()
    {
        return object::acquire1<hash_meta_node>();
    }

    void hash_meta_node:: release(hash_meta_node *node) throw()
    {
        assert(node);
        object::release1(node);
    }


    
}

