
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/hashing/sha1.hpp"
#include "y/utest/timings.hpp"
#include "y/string.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;


Y_UTEST(xbra)
{
    const string header_name = "xbra.hpp";
    const string source_name = "xbra.cpp";

    ios::ocstream header(header_name);
    ios::ocstream source(source_name);


    size_t         pmax = 8;
    vector<size_t> I(pmax,as_capacity);
    vector<size_t> J(pmax,as_capacity);

    for(size_t p=0;p<=pmax;++p)
    {
        I.free();
        J.free();
        const size_t size = 1<<p;
        const size_t n    = size << 1;
        size_t       j    = 1;
        for( size_t i=1; i<n; i+=2)
        {
            if(j>i)
            {
                //Y_XBITREV_SWAP(i,j);
                I.push_back_(i);
                J.push_back_(j);
            }
            size_t m = size;
            while( (m>=2) && (j>m) )
            {
                j -=  m;
                m >>= 1;
            }
            j += m;
        }
    }



}
Y_UTEST_DONE()

