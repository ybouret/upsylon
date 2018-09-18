#include "y/memory/slab.hpp"
#include "y/utest/run.hpp"
#include "y/memory/buffers.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    class Node : public object
    {
    public:
        Node *next;
        Node *prev;
        T    *addr;
        inline explicit Node(T *p) throw() : next(0), prev(0), addr(p) {}
        inline virtual ~Node() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Node);
    };

    struct dummy
    {
        uint32_t a;
        uint32_t b;
        uint32_t c;
    };

    template <typename T>
    static inline void do_test()
    {
        std::cerr << std::endl;
        std::cerr << "slab<size=" << sizeof(T) << ">" << std::endl;
        std::cerr << "\tblock_size=" << memory::slab<T>::block_size << std::endl;
        core::list_of_cpp< Node<T> > nodes;

        for(size_t count=1;count<=1024;++count)
        {
            const size_t bytes = memory::slab<T>::bytes_for(count);
            memory::buffer_of<char,memory::global> buff(bytes);
            memory::slab<T> S(*buff,bytes);
            while( S.size( ) )
            {
                T *addr = S.acquire();
                try
                {
                    nodes.push_back( new Node<T>(addr) );
                }
                catch(...)
                {
                    S.release(addr);
                    throw;
                }
            }
            alea.shuffle(nodes);
            while( nodes.size )
            {
                Node<T> *node = nodes.pop_back();
                S.release(node->addr);
                delete node;
            }
        }
    }
}

Y_UTEST(slab)
{
    do_test<uint8_t>();
    do_test<uint16_t>();
    do_test<uint32_t>();
    do_test<uint64_t>();
    do_test<dummy>();
}
Y_UTEST_DONE()
