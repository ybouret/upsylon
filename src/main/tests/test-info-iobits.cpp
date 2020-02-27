#include "y/information/iobits.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/string.hpp"
#include "y/string/io.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Information;

static inline size_t sumOf(const accessible<size_t> &sz )
{
    size_t ans = 0;
    for(size_t i=sz.size();i>0;--i) ans += sz[i];
    return ans;
}

Y_UTEST(IOBits)
{
    Y_UTEST_SIZEOF(bool);
    IOBits Q;


    {
        std::cerr << "-- checking random packing" << std::endl;
        vector<uint8_t>  v8;
        vector<size_t>   n8;

        vector<uint16_t> v16;
        vector<size_t>   n16;

        vector<uint32_t> v32;
        vector<size_t>   n32;

        vector<uint64_t> v64;
        vector<size_t>   n64;


        for(size_t i=0;i<100+alea.leq(1000);++i)
        {
            {
                const size_t  bits8 = alea.leq(8);
                const uint8_t word8 = alea.partial<uint8_t>(bits8);
                v8 << word8;
                n8 << bits8;
                Q._push<uint8_t>(word8,bits8);
            }

            {
                const size_t   bits16 = alea.leq(16);
                const uint16_t word16 = alea.partial<uint16_t>(bits16);
                v16 << word16;
                n16 << bits16;
                Q._push<uint16_t>(word16,bits16);
            }

            {
                const size_t   bits32 = alea.leq(32);
                const uint32_t word32 = alea.partial<uint32_t>(bits32);
                v32 << word32;
                n32 << bits32;
                Q._push<uint32_t>(word32,bits32);
            }

            {
                const size_t   bits64 = alea.leq(64);
                const uint64_t word64 = alea.partial<uint64_t>(bits64);
                v64 << word64;
                n64 << bits64;
                Q._push<uint64_t>(word64,bits64);
            }

        }

        Y_CHECK( v8.size()  == n8.size()  );
        Y_CHECK( v16.size() == n16.size() );
        Y_CHECK( v32.size() == n32.size() );
        Y_CHECK( v64.size() == n64.size() );

        Y_CHECK( v8.size() == v16.size() );
        Y_CHECK( v8.size() == v32.size() );
        Y_CHECK( v8.size() == v64.size() );

        Y_CHECK( sumOf(n8) + sumOf(n16) + sumOf(n32) + sumOf(n64) == Q.size() );
        std::cerr << "#bits=" << Q.size() << std::endl;
        Q.zpad( alea );
        std::cerr << "#bits=" << Q.size() << std::endl;


        const size_t n = v8.size();
        for(size_t i=1;i<=n;++i)
        {
            Y_ASSERT(Q.size()>=n8[i]);  Y_ASSERT( v8[i]  == Q._pop<uint8_t>(n8[i]) );
            Y_ASSERT(Q.size()>=n16[i]); Y_ASSERT( v16[i] == Q._pop<uint16_t>(n16[i]) );
            Y_ASSERT(Q.size()>=n32[i]); Y_ASSERT( v32[i] == Q._pop<uint32_t>(n32[i]) );
            Y_ASSERT(Q.size()>=n64[i]); Y_ASSERT( v64[i] == Q._pop<uint64_t>(n64[i]) );
        }

        Q.drop();
        Y_CHECK( 0 == Q.size() );
    }

    
    std::cerr << "-- checking serialization" << std::endl;
    {
        const string s = "Hello, World!";
        const size_t n = s.serialize(Q);
        Q.flush();
        Y_CHECK( n * 8 == Q.size() );
        const string r = string_io::load_binary(Q);
        std::cerr << "send=" << s << ", recv=" << r << std::endl;
        Y_CHECK(s==r);
    }

    std::cerr << "-- checking store/query" << std::endl;
    for(size_t i=0;i<256;++i)
    {
        const uint8_t u = uint8_t(i);
        const char    c(u);
        Q.store(c);
        char C = 0;
        Y_ASSERT( Q.query(C) && c == C );
    }
    std::cerr << "all bytes are ok" << std::endl;


    std::cerr << "-- checking any integral type" << std::endl;
#define Y_QBITS_IO(TYPE) do {            \
const TYPE a_##TYPE = alea.full<TYPE>(); \
Q.push(a_##TYPE);                        \
const TYPE b_##TYPE = Q.pop<TYPE>();     \
Y_ASSERT( a_##TYPE == b_##TYPE );        \
} while(false)


    typedef long long          my_ilong;

    for(size_t iter=0;iter<100;++iter)
    {
        Y_QBITS_IO(char);
        Y_QBITS_IO(short);
        Y_QBITS_IO(int);
        Y_QBITS_IO(long);
        Y_QBITS_IO(my_ilong);
    }

    size_t       code = 0;
    vector<bool> ibits;
    const size_t nbit = alea.leq(sizeof(code));
    for(size_t i=0;i<nbit;++i)
    {
        const bool b = alea.choice();
        code <<= 1;
        if(b) code |= 1;
        ibits.push_back(b);
    }

    {
        std::cerr << "-- Building Flags Table" << std::endl;
        ios::ocstream fp( "iobytes.bin" );
        bool flags[256][8];
        memset(flags,0,sizeof(flags));
        std::cerr << "sizeof(flags)=" << sizeof(flags) << std::endl;
        for(unsigned i=0;i<256;++i)
        {
            Q.free();
            const uint8_t byte = uint8_t(i);
            Q.push(byte);
            Y_ASSERT(Q.size()==8);
            //std::cerr << "Q[" << i << "]=" << Q << std::endl;
            fp << '{';
            size_t j=0;
            for( IOBits::reverse_iterator it=Q.rbegin(); j<8; ++j, ++it)
            {
                const bool code = *it;
                flags[i][j] = code;
                if(code)
                {
                    fp << "  true";
                }
                else
                {
                    fp << " false";
                }
                if(j<7) fp << ',';
            }
            fp << ' ' << '}';
            if(i<255) fp << ','; else fp << ' ';
            fp(" // 0x%02x = %u", i,i);
            fp  << '\n';
        }

        std::cerr << "-- Testing Flags Table" << std::endl;
        Q.release();
        Q.guarantee(8);

        for(unsigned i=0;i<256;++i)
        {
            Q.free();
            for(unsigned j=0;j<8;++j)
            {
                Q.push_front_( flags[i][j] );
            }
            Y_ASSERT(Q.pop<uint8_t>() == i );
        }

    }

}
Y_UTEST_DONE()




