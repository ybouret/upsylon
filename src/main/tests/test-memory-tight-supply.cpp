#include "y/memory/tight/supply.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace memory;

namespace {

    struct dnode
    {
        short    a;
    };

    class dnode_supply : public tight::supply_of<dnode>, public singleton< dnode_supply >
    {
    public:

        void reserve(const size_t n) throw()
        {
            Y_LOCK(access);
            fetch(n);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dnode_supply);
        Y_SINGLETON_DECL(dnode_supply);
        explicit dnode_supply()
        {
        }

        virtual ~dnode_supply() throw()
        {

        }
    };

    Y_SINGLETON_IMPL_WITH(0,dnode_supply);

}

Y_UTEST(tight_supply)
{
    concurrent::singleton::verbose = true;
    dnode_supply &drepo = dnode_supply::instance();

    drepo.reserve(100);

    std::cerr << "block_size=" << drepo.block_size   << std::endl;
    std::cerr << "prefetched=" << drepo.prefetched() << std::endl;
}
Y_UTEST_DONE()

