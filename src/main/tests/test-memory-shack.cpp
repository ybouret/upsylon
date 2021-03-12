
#include "y/memory/shacks.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void do_shacks( memory::shacks &S )
    {

        S.make<T>();
        for(size_t i=0;i<S.size();++i)
        {
            memory::shack &s = *S[i];
            Y_ASSERT(s.is<T>());

            addressable<T> &arr = s.__<T>();
            std::cerr << "#arr=" << arr.size() << std::endl;
            Y_ASSERT(1==arr.size());
        }

        const size_t r = 1+alea.leq(20);
        S.make<T>(r);
        for(size_t i=0;i<S.size();++i)
        {
            memory::shack &s = *S[i];
            Y_ASSERT(s.is<T>());

            addressable<T> &arr = s.__<T>();
            std::cerr << "#arr=" << arr.size() << std::endl;
            Y_ASSERT(r==arr.size());
            //T *ptr = &s._<T>();
        }




    }

}

Y_UTEST(shack)
{
    {
        memory::shack S;
        for(size_t i=0;i<=10000;i += alea.leq(100) )
        {

            S.acquire(i);
            Y_ASSERT(S.is_anonymous());
            Y_ASSERT(S.is_zeroed());
            for(size_t j=0;j<8;++j)
            {
                S.release();
                S.acquire(i);
            }
        }


        S.release();
        S.acquire(1);
        Y_CHECK(S.is_anonymous());
        Y_CHECK(S.tell()==0);
        S.make<string>() = "hello";
        Y_CHECK(S.is_cplusplus());
        Y_CHECK(S.is<string>());
        Y_CHECK(S.tell()==1);

        S.make<string>(7);
        Y_CHECK(S.is_cplusplus());
        Y_CHECK(S.is<string>());
        Y_CHECK(S.tell()==7);

        Y_UTEST_SIZEOF(memory::shack);
        Y_UTEST_SIZEOF(lightweight_array<int>);
        Y_UTEST_SIZEOF(lightweight_array<string>);
        Y_UTEST_SIZEOF(lightweight_array<char>);

        support::fill1D( S.__<string>() );
        std::cerr << S.__<string>() << std::endl;

        {
            const string a = "hello";
            S.build<string>(a);
            std::cerr << S._<string>() << std::endl;
        }

        {
            S.build<string>( "world" );
            std::cerr << S._<string>() << std::endl;
        }
    }

    {
        memory::shacks S(10,memory::shacks::construct_filled);
        std::cerr << "S.size=" << S.size() << std::endl;

        do_shacks<short>(S);
        do_shacks<double>(S);


    }

}
Y_UTEST_DONE()
