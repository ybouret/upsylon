#include "y/ios/ocstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;



Y_UTEST(ostreams)
{
    {
        const string fn = "echo.dat";
        ios::ocstream::overwrite(fn);
        int i=0;
        ios::ocstream::echo(fn,"Hello, %d\n",++i);
        ios::ocstream::echo(fn,"Hello, %d\n",++i);
        ios::ocstream::echo(fn,"Hello, %d\n",++i);
        ios::ocstream fp(fn,true);
        fp << fn;
        fp << '\n';
        fp << "Hello, World!" << "\n";
        fp("Hello, %d\n", ++i);
    }

    {
        string output;
        {
            ios::osstream fp(output);
            int i=0;
            fp("Hello, %d\n",++i);
            fp("Hello, %d\n",++i);
            fp("Hello, %d\n",++i);
            fp << "World" << '\n';
        }
        std::cerr << output << std::endl;
    }

    {
        vector<uint64_t> data;

        string s(16,as_capacity,false);
        {
            ios::ocstream op("upack.dat");

            std::cerr << "-- first 32" << std::endl;
            for(size_t i=0;i<=32;++i)
            {
                s.clear();
                {
                    ios::osstream fp(s);
                    fp.write_upack(i);
                    op.write_upack(i);
                    data << i;
                }
                {
                    ios::imstream inp(s);
                    size_t j = 0;
                    Y_ASSERT( inp.query_upack(j) );
                    std::cerr << "\ti=" << i << " -> j=" << j << std::endl;
                    Y_ASSERT(j==i);
                }
            }

#define N 1000
            std::cerr << "-- alea 16bits" << std::endl;
            for(size_t i=0;i<N;++i)
            {
                s.clear();
                ios::osstream fp(s);
                const uint16_t x = alea.partial<uint16_t>();
                fp.write_upack<uint64_t>(x);
                op.write_upack<uint64_t>(x);
                data << x;
                {
                    ios::imstream inp(s);
                    uint16_t j = 0;
                    Y_ASSERT(inp.query_upack(j));
                    std::cerr << "\tx=" << x << " -> j=" << j << std::endl;
                    Y_ASSERT(j==x);
                }
            }

            std::cerr << "-- alea 32bits" << std::endl;
            for(size_t i=0;i<N;++i)
            {
                s.clear();
                ios::osstream fp(s);
                const uint32_t x = alea.partial<uint32_t>();
                fp.write_upack<uint64_t>(x);
                op.write_upack<uint64_t>(x);
                data << x;
                {
                    ios::imstream inp(s);
                    uint32_t j = 0;
                    Y_ASSERT(inp.query_upack(j));
                    std::cerr << "\tx=" << x << " -> j=" << j << std::endl;
                    Y_ASSERT(j==x);
                }
            }

            std::cerr << "-- alea 64bits" << std::endl;
            for(size_t i=0;i<N;++i)
            {
                s.clear();
                ios::osstream fp(s);
                const uint64_t x = alea.partial<uint64_t>();
                fp.write_upack<uint64_t>(x);
                op.write_upack<uint64_t>(x);
                data << x;
                {
                    ios::imstream inp(s);
                    uint64_t j = 0;
                    Y_ASSERT(inp.query_upack(j));
                    std::cerr << "\tx=" << x << " -> j=" << j << std::endl;
                    Y_ASSERT(j==x);
                }
            }


        }

        {
            std::cerr << "Reading Upacked..." << std::endl;
            ios::icstream fp("upack.dat");
            for(size_t i=1;i<=data.size();++i)
            {
                uint64_t loaded = 0;
                Y_ASSERT(fp.query_upack(loaded));
                Y_ASSERT(loaded==data[i]);
            }
        }

        int i=0;
        const void *addr = &i;
        union
        {
            const void *addr;
            uint8_t     byte[ sizeof(void*) ];
        } alias = { addr };

        std::cerr << "addr=" << addr << std::endl;
        std::cerr << "byte=0x"; for(size_t i=0;i<sizeof(void*);++i) std::cerr << hexadecimal::lowercase[ alias.byte[i] ];
        std::cerr << std::endl;

    }

}
Y_UTEST_DONE()


