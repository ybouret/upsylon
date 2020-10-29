#include "y/associative/prefix/stem.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void dispText()
    {
        std::cerr << type_name_of<T>() << std::endl;
        for(size_t i=0;i<8;++i)
        {
            const T x = alea.full<T>();
            std::cerr << '\t' << prefix_::code_to_text(x) << std::endl;
        }

    }

    template <typename CODE, typename T>
    static inline void _disp(const bool save=false)
    {
        typedef prefix_node<CODE,T> node_type;
        std::cerr << "node<" << type_name_of<CODE>() << "," << type_name_of<T>() << "> : " << sizeof(node_type) << std::endl;

        auto_ptr<node_type>            root( new node_type(0,0,0) );
        typename node_type::list_t    &leaves = root->leaves;
        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            leaves.push_back( new node_type(0,CODE(i),0) );
            leaves.tail->frequency = alea.range<size_t>(1,1000);
        }
        root->optimize();
        for(const node_type *node = leaves.head;node;node=node->next)
        {
            if(node->next)
            {
                Y_ASSERT(node->frequency>=node->next->frequency);
            }
        }
        if(save)
        {
            root->graphViz("node.dot",true);
        }
    }

    template <typename T>
    static inline void disp()
    {
        _disp<uint8_t,T>();
        _disp<uint16_t,T>();
        _disp<uint32_t,T>();
        _disp<uint64_t,T>();
    }


    template <typename CODE, typename T>
    void doStem()
    {
        prefix_stem<CODE,T> stem;

    }

}

Y_UTEST(prefix)
{
    dispText<char>();
    dispText<short>();
    dispText<int>();
    dispText<size_t>();

    disp<null_type>();
    disp<int>();

    _disp<uint8_t,null_type>(true);

    doStem<int,null_type>();

}
Y_UTEST_DONE()


