#include "y/suffix/node.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
namespace
{
    template <typename CODE>
    void dispNode()
    {
        std::cerr << "-- suffix_node<" << type_name_of<CODE>() << ">" << std::endl;
        typedef suffix_node<CODE> node_type;
        Y_UTEST_SIZEOF(node_type);
    }
}
Y_UTEST(suffix)
{
    dispNode<uint8_t>();
    dispNode<uint16_t>();
    dispNode<uint32_t>();
    dispNode<uint64_t>();

}
Y_UTEST_DONE()


