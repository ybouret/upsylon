#include "y/ordered/avl-tree.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void display( const T &data, const size_t level )
    {
        for(size_t i=0;i<level;++i) std::cerr << ".";
        std::cerr << "[" << data << "]" << std::endl;
    }
}

Y_UTEST(avl)
{

    avl::tree<int>    itree;

    for(size_t i=10;i>0;--i)
    {
        itree.insert( alea.partial<int>(10) );
    }
    std::cerr << "inorder" << std::endl;
    itree.inorder(display<int>);
    std::cerr << "preorder" << std::endl;
    itree.preorder(display<int>);
    std::cerr << "postorder" << std::endl;
    itree.postorder(display<int>);

}
Y_UTEST_DONE()



