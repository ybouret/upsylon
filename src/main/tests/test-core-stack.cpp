
#include "y/core/stack.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

#include "y/sequence/vector.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline void testStack()
    {
        std::cerr << type_name_of<T>() << std::endl;
        Y_DECL_ARGS_(T,type);

        const size_t n = 1 + alea.leq(30);
        vector<type>          data(n,as_capacity);
        vector<mutable_type*> meta(n,NULL);
        for(size_t i=0;i<n;++i)
        {
            const_type tmp = support::get<mutable_type>();
            data.push_back_(tmp);
        }

        core::stack<type> stk( (type **) *meta, n );
        for(size_t i=1;i<=n;++i)
        {
            stk.push( &data[i] );
        }

        for(size_t i=1;i<=n;++i)
        {
            const_type &tmp = stk.peek();
            const_type &ref = *stk.pop();
            Y_ASSERT(tmp==ref);
            std::cerr << "\t" << ref << std::endl;
        }
        std::cerr << std::endl;
    }

}

Y_UTEST(core_stack)
{
    testStack<short>();
    testStack<const short>();
    testStack<string>();
    testStack<const mpn>();
}
Y_UTEST_DONE()

