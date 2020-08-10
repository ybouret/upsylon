
#include "y/memory/tight/object.hpp"
#include "y/memory/tight/dyadic-allocator.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include <iomanip>
#include <cstring>

using namespace upsylon;
using namespace memory;

namespace {
    typedef tight::object<4096,512> obj_type;

    template <size_t N>
    class dummy : public obj_type
    {
    public:
        char data[N];

        explicit dummy() throw() : data()
        {
            memset(data,0,sizeof(data));
        }

        virtual ~dummy() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

    template <size_t N> static inline
    void testDummy()
    {
        typedef dummy<N> dum_type;

        std::cerr << "sizeof( dummy<" << std::setw(3) <<  N << ">) = " << sizeof(dum_type) << std::endl;
        {
            dum_type *d = new dum_type();
            delete    d;
            d = NULL;
            delete d;
        }

        {
            dum_type *d = new dum_type[10];
            delete []d;
            d= NULL;
            delete []d;
        }

    }

    struct blk_t
    {
        void  *addr;
        size_t size;
    };

}


Y_UTEST(tight_object)
{

    obj_type::supply &mgr = obj_type::supply::instance();
    std::cerr << mgr.Quarry << std::endl;

    testDummy<1>();
    testDummy<10>();

    const size_t exp2_last=10;
    const size_t count    = 16;

    {
        void *addr[exp2_last][count];

        memset(addr,0,sizeof(addr));
        for(size_t exp2=0;exp2<exp2_last;++exp2)
        {
            for(size_t j=0;j<count;++j)
            {
                addr[exp2][j] = mgr.dyadic_acquire(exp2);
            }
            alea.shuffle(addr[exp2],count);
        }
        for(size_t exp2=0;exp2<exp2_last;++exp2)
        {
            for(size_t j=0;j<count;++j)
            {
                mgr.dyadic_release(addr[exp2][j],exp2);
            }
        }
    }

    std::cerr << mgr.Quarry << std::endl;

    {
        tight::dyadic_allocator D(mgr);
        blk_t        reg[32];
        const size_t num = sizeof(reg)/sizeof(reg[0]);

        for(size_t i=0;i<num;++i)
        {
            reg[i].addr = 0;
            reg[i].size = alea.leq(10000);
        }
        alea.shuffle(reg, num);
        for(size_t i=0;i<num;++i)
        {
            std::cerr << std::setw(5)<< reg[i].size;
            reg[i].addr = D.acquire( reg[i].size );
            std::cerr << " -> " << std::setw(5) << reg[i].size << std::endl;
        }
        alea.shuffle(reg,num);
        for(size_t i=0;i<num;++i)
        {
            D.release(reg[i].addr, reg[i].size);
        }

    }

    std::cerr << mgr.Quarry << std::endl;


}
Y_UTEST_DONE()


