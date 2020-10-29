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
    static inline void _disp()
    {
        typedef prefix_node<CODE,T> node_type;
        std::cerr << "node<" << type_name_of<CODE>() << "," << type_name_of<T>() << "> : " << sizeof(node_type) << std::endl;

#if 0
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
#endif

    }

    template <typename T>
    static inline void disp()
    {
        _disp<uint8_t,T>();
        _disp<uint16_t,T>();
        _disp<uint32_t,T>();
        _disp<uint64_t,T>();
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

}
Y_UTEST_DONE()


