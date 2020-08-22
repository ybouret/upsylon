#include "y/sequence/arrays.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>
#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void do_test()
    {

        for(size_t iter=0;iter<10;++iter)
        {
            arrays<T> arr(1+alea.leq(30));
            std::cerr << "#arr<" << typeid(T).name() << ">=" << arr.count << std::endl;
            std::cerr << "\tsize=" << arr.size() << std::endl;
            for(size_t jter=0;jter<10;++jter)
            {
                arr.acquire(1+alea.leq(100));
                std::cerr << "\tsize=" << arr.size() << std::endl;
                for(size_t i=0;i<arr.count;++i)
                {
                    array<T> &a = arr[i];
                    Y_ASSERT(a.size()==arr.size());
                    for(size_t j=a.size();j>0;--j)
                    {
                        a[j] = support::get<T>();
                    }
                }
            }
        }
    }

}
Y_UTEST(arrays)
{
    do_test<int>();
    do_test<double>();
    do_test<string>();
    do_test<apq>();
}
Y_UTEST_DONE()

