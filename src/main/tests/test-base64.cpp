#include "y/codec/base64.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/type/utils.hpp"
#include <typeinfo>
#include "y/codec/base64-name.hpp"

using namespace upsylon;

template <typename T>
static inline void test_b64id()
{
    std::cerr << "base64_name<" << typeid(T).name() << "/#bytes=" << sizeof(T) << ">" << std::endl;
    std::cerr << "\treq_bytes=" << base64_name<T>::req_bytes << std::endl;
    std::cerr << "\trnd_bytes=" << base64_name<T>::rnd_bytes << std::endl;
    std::cerr << "\tsizeof   =" << sizeof(base64_name<T>)    << std::endl;
    Y_ASSERT(sizeof(base64_name<T>)== base64_name<T>::rnd_bytes);
    for(T i=0;i<=8;++i)
    {
        const T x = i*i;
        base64_name<T> id = x;
        std::cerr << std::hex << "\t\tx=" << x << " -> " << std::dec << *id << std::endl;
    }
    for(size_t i=0;i<8;++i)
    {
        const T x = alea.full<T>();
        base64_name<T> id = x;
        std::cerr << std::hex << "\t\tx=" << x << " -> " << std::dec << *id << std::endl;
    }


}


Y_UTEST(b64id)
{
    test_b64id<unsigned char>();
    test_b64id<char>();

    test_b64id<short>();
    test_b64id<unsigned short>();

    test_b64id<int>();
    test_b64id<unsigned int>();

    test_b64id<long>();
    test_b64id<unsigned long>();

    base64_name<float>  idf = 0.3f; std::cerr << "idf=" << *idf << std::endl;
    base64_name<double> idd = 0.3;  std::cerr << "idd=" << *idd << std::endl;


}
Y_UTEST_DONE()

Y_UTEST(base64)
{

    if( (argc>1) && (0==strcmp(argv[1],"NULL")) )
    {

        // build inverse table
        std::cerr << "Look Up: " << std::endl;
        for(size_t i=0;i<256;++i)
        {
            const char C = char(i);
            const int  std_code = ios::base64::find_in_std(C);
            const int  url_code = ios::base64::find_in_url(C);
            if(url_code!=std_code)
            {
                //std::cerr << "#" << i << " -> " << std_code << "|" << url_code << std::endl;
                Y_ASSERT(url_code==-1||std_code==-1);
            }
            const int value = max_of(std_code,url_code);
            fprintf(stdout," %2d",value);
            if(i<255) fprintf(stdout,",");
            if( 0==((1+i)&15) ) fprintf(stdout,"\n");
        }
        return 0;
    }

    std::cerr << "Encoding..." << std::endl;
    ios::icstream        fp( ios::cstdin  );
    ios::ocstream        op( ios::cstdout );
    ios::base64::encoder b64;

    b64.filter(op,fp);

}
Y_UTEST_DONE()

Y_UTEST(u64)
{
    ios::icstream        fp( ios::cstdin  );
    ios::ocstream        op( ios::cstdout );
    ios::base64::decoder u64;
    std::cerr << "Decoding..." << std::endl;
    u64.filter(op,fp);
}
Y_UTEST_DONE()

