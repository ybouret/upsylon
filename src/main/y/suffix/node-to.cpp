#include "y/suffix/node-to.hpp"


namespace upsylon {

    string suffix_node_::to_string(const suffix_node<char> *node)
    {
        assert(node);
        string s(node->depth,as_capacity,true); assert(s.size()==node->depth);
        node->encode(*s);
        return s;
    }

    void  * suffix_node_:: to_address(const suffix_node<char> *node)
    {
        assert(node);
        assert(sizeof(void*)==node->depth);
        union
        {
            unsigned_int<sizeof(void*)>::type word;
            char                              path[sizeof(void*)];
            void                             *addr;
        } alias = { 0 };
        node->encode(alias.path);
        alias.word = swap_be(alias.word);
        return alias.addr;
    }


}
