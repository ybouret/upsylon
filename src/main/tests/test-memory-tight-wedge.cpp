#include "y/memory/tight/wedge.hpp"
#include "y/utest/run.hpp"
#include <cstring>
#include "y/type/aliasing.hpp"

using namespace upsylon;
using namespace memory;

namespace {

    template <size_t N>
    class dummy
    {
    public:
        char data[N];

        dummy() throw() : data()
        {
            memset(data,0,sizeof(data));
        }

        ~dummy() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };



    template <size_t N>
    void testWedge()
    {
        typedef dummy<N> type;
        std::cerr << "sizeof(type)=" << sizeof(type) << std::endl;
        typedef tight::wedge<type> wedge;
        std::cerr << "wedge.block_size=" << wedge::block_size << std::endl;
        std::cerr << "wedge.block_step=" << wedge::block_step << std::endl;

        char  data[1024];
        type *reg[sizeof(data)];

        wedge w(data,sizeof(data));

        std::cerr << "w.provided_number=" << w.provided_number << std::endl;
        Y_CHECK(w.provided_number<=sizeof(data));
        static const size_t in_use   = aliasing::delta(w.data,w.last);
        static const size_t computed =w.provided_number * w.block_size;
        Y_CHECK( in_use == computed );

        for(size_t iter=0;iter<8;++iter)
        {
            size_t num = 0;
            std::cerr << '[';
            while( w.still_available )
            {
                std::cerr << '+';
                reg[num++] = w.acquire();
            }
            std::cerr << ']';
            std::cerr << std::endl;
            alea.shuffle(reg,num);
            std::cerr << '[';
            for(size_t i=0;i<num;++i)
            {
                std::cerr << '-';
                w.release(reg[i]);
            }
            std::cerr << ']';
            std::cerr << std::endl;
        }

        std::cerr << std::endl;
    }

}

Y_UTEST(tight_wedge)
{
    testWedge<1>();
    testWedge<2>();
    testWedge<3>();
    testWedge<4>();
    testWedge<5>();
    testWedge<6>();
    testWedge<7>();
    testWedge<8>();
    testWedge<9>();
    testWedge<10>();
    testWedge<11>();
    testWedge<12>();

}
Y_UTEST_DONE()

