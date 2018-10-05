#include "y/codec/iobits.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(iobits)
{
    std::cerr << "sizeof(iobit)=" << sizeof(iobit) << std::endl;
    iobits Q;
    for(size_t i=1000+alea.leq(10000);i>0;--i)
    {
        Q.push( alea.choice() );
    }

    Q.free();
    Q.pool.keep(256);

    if( false )
    {
        std::cerr << std::hex;
        uint64_t u = 1;
        for(size_t i=0;i<8;++i)
        {
            for(size_t j=0;j<8;++j)
            {
                std::cerr << "Y_U64(0x" << u << "), ";
                u <<= 1;
            }
            std::cerr << std::endl;
        }
    }
    else
    {
        vector<uint64_t>  values;
        vector<size_t>    vbits;

        for(size_t iter=0;iter<10;++iter)
        {
            size_t            total = 0;
            Q.free();
            values.free();
            vbits.free();
            for( size_t i= 20 + alea.leq(20); i>0; --i)
            {
                const uint64_t u = alea.partial<uint64_t>();
                const size_t   n = bits_for(u);
                values.push_back(u);
                vbits.push_back(n);
                Q.push(u,n);
                total+=n;
            }
            std::cerr << "values=" << values << std::endl;
            std::cerr << "vbits =" << vbits  << std::endl;
            Y_CHECK(Q.size==total);
            const size_t nv = values.size();
            for(size_t i=1;i<=nv;++i)
            {
                const size_t    n = vbits[i]; Y_ASSERT(Q.size>=n);
                const uint64_t  u = Q.pop<uint64_t>(n);
                //std::cerr << "u=" << u << "/" << values[i] << std::endl;
                Y_ASSERT(u==values[i]);
            }
        }
    }
}
Y_UTEST_DONE()

