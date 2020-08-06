
#include "y/memory/small/object.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

namespace {
    typedef small::object<4096,512> obj_type;

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
        std::cerr << small::chunk::max_chunk_size_for(sizeof(dum_type)) << std::endl;
        dum_type *d = new dum_type();
        (void)d;
    }

}


Y_UTEST(small_object)
{

    obj_type::supply &mgr = obj_type::supply::instance();
    std::cerr << mgr.Quarry << std::endl;

    testDummy<1>();
    testDummy<10>();

    std::cerr << mgr.Quarry << std::endl;


}
Y_UTEST_DONE()


