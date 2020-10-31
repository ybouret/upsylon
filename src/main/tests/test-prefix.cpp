#include "y/associative/prefix/depot.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/buffers.hpp"

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
        std::cerr << "in_use_addr: " << (void*) ( prefix_data<T>::in_use_addr() ) << std::endl;
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
            root->graphViz("node.dot");
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
        std::cerr << "-- stem<" << type_name_of<CODE>() << "," << type_name_of<T>() << ">" << std::endl;
        typedef prefix_stem<CODE,T>           stem_type;
        typedef typename stem_type::node_type node_type;
        typedef vector<CODE> path_type;
        
        stem_type       stem;
        list<T>         data;
        list<path_type> paths;
        size_t          count = 0;
        
        std::cerr << "building:[";
        for(size_t iter=80;iter>0;--iter)
        {
            vector<CODE> path;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                path << alea.range<CODE>('a','d');
            }
            data << support::get<T>();
            node_type  *mark = 0;
            node_type  *node = stem.grow( path.begin(), path.size(), & data.back(), &mark);
            if(node)
            {
                Y_ASSERT(mark==node);
                std::cerr << "+";
                ++count;
                Y_ASSERT(stem.tell()==count);
                paths << path;
            }
            else
            {
                std::cerr << "-";
                data.pop_back();
            }
        }
        std::cerr << "]" << std::endl;
        stem.get_root().graphViz("stem.dot");
        Y_CHECK(count==stem.tell());
        std::cerr << "duplicate" << std::endl;
        {
            stem_type stem2;
            stem2.duplicate(stem);
            Y_CHECK(stem2.similar_to(stem));
        }
        
        alea.shuffle( *paths );
        std::cerr << "removing:[";
        while(paths.size())
        {
            const path_type &path = paths.back();
            const node_type *node = stem.find( path.begin(), path.size() );
            Y_ASSERT(node);
            Y_ASSERT(node->addr);
            Y_ASSERT(node->depth==path.size());
            memory::cppblock<CODE> blk(node->depth);
            node->encode(blk);
            for(size_t i=1;i<=node->depth;++i)
            {
                Y_ASSERT(blk[i]==path[i]);
            }
            paths.pop_back();
            stem.pull((node_type *)node);
            Y_ASSERT(stem.tell()==paths.size());
            std::cerr << "-";
        }
        std::cerr << "]" << std::endl;
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

    doStem<uint16_t,int>();
   
}
Y_UTEST_DONE()


