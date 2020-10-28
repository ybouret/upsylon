#include "y/associative/prefix/stump.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;

namespace
{
    template <typename CODE,typename T>
    static inline void _disp()
    {
        typedef prefix_node<CODE,T> node_type;
        std::cerr << "node<" << type_name_of<CODE>() << "," << type_name_of<T>() << "> :" << sizeof(node_type) << std::endl;
        
        auto_ptr<node_type>            root( new node_type(0,0,0) );
        typename node_type::list_type &leaves = root->leaves;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            leaves.push_back( new node_type(0,CODE(i),0) );
            leaves.tail->freq = 0;
        }
        root->optimize();
        for(const node_type *node = leaves.head;node;node=node->next)
        {
            if(node->next)
            {
            }
        }
    }
    
    template <typename T>
    static inline void disp()
    {
        _disp<int8_t,T>();
        _disp<int16_t,T>();
        _disp<int32_t,T>();
        _disp<int64_t,T>();
        std::cerr << std::endl;

    }
    
}

Y_UTEST(prefix)
{
    
    disp<int8_t>();
    disp<int16_t>();
    disp<int32_t>();
    disp<int64_t>();
    
    disp<void *>();
    
    
}
Y_UTEST_DONE()


