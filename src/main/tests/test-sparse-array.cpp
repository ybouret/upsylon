#include "y/sparse/array.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

template <typename SP_ARR>
static inline void query( const SP_ARR &arr )
{

    std::cerr << "iter: ";
    for(typename SP_ARR::const_iterator i = arr.begin(); i != arr.end(); ++i )
    {
        std::cerr << ' ' << *i;
    }
    std::cerr << std::endl;
}

template <typename SP_ARR>
static inline void fill( SP_ARR &arr )
{
    std::cerr << "-- fill and ops" << std::endl;
    for(size_t i=arr.size()/2;i>0;--i)
    {
        const typename SP_ARR::type tmp = support::get< typename SP_ARR::mutable_type >();
        const size_t j = 1 + alea.lt(arr.size());
        arr[j] = tmp;
    }
    query(arr);
    std::cerr << "arr = " << arr << " #" << arr.core.size() << "/" << arr.size() << std::endl;
    arr.set_size( arr.size()/2 );
    query(arr);
    arr.update();
    query(arr);
    std::cerr << "arr = " << arr << " #" << arr.core.size() << "/" << arr.size() << std::endl;


    std::cerr << std::endl;
}



Y_UTEST(sparse_array)
{

    if(false)
    {
        sparse::dok<size_t,string> sm;
        sparse::dok<size_t,double> sd(8);
        for(size_t i=1;i<=16;++i)
        {
            const string svalue = support::get<string>();
            const double dvalue = support::get<double>();
            Y_ASSERT(sm.create(i*i,svalue));
            Y_ASSERT(sd.create(i*i,dvalue));
        }
        std::cerr << "sm=" << sm << std::endl;
        std::cerr << "sd=" << sd << std::endl;

        sd.sort_keys( comparison::decreasing<size_t> );
        std::cerr << "sd=" << sd << std::endl;

        sparse::dok<size_t,double> sd2(sd);
        sd2.sort_keys( comparison::increasing<size_t> );
        std::cerr << "sd2=" << sd2 << std::endl;
    }

    sparse_array<mpq>    as(10);
    sparse_array<double> ad(12);

    fill(as);
    fill(ad);


}
Y_UTEST_DONE()


