#include "y/core/rnode.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(core_rnode)
{
    core::list_of_cpp< ref_node<int,dnode> >       vri;
    core::list_of_cpp< ref_node<const int,dnode> > cri;

    int arr[5] = { 0 };
    for(unsigned i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
    {
        arr[i] = i;
        vri.push_back( new ref_node<int,dnode>(arr[i]) );
        cri.push_back( new ref_node<const int,dnode>(arr[i]));
    }

    alea.shuffle(vri);
    alea.shuffle(cri);
    for(const ref_node<int,dnode> *node=vri.head;node;node=node->next)
    {
        std::cerr << **node << std::endl;
    }
    for(const ref_node<const int,dnode> *node=cri.head;node;node=node->next)
    {
        std::cerr << **node << std::endl;
    }

}
Y_UTEST_DONE()

