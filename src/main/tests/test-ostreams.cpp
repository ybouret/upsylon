#include "y/ios/ocstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"

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

        string s(16,as_capacity);
        {
            ios::ocstream op("upack.dat");

            std::cerr << "-- first 32" << std::endl;
            for(size_t i=0;i<=32;++i)
            {
                s.clear();
                {
                    ios::osstream fp(s);
                    fp.emit_upack(i);
                    op.emit_upack(i);
                    data << i;
                }
                {
                    ios::imstream inp(s);
                    const size_t j = inp.read_upack<size_t>();
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
                fp.emit_upack<uint64_t>(x);
                op.emit_upack<uint64_t>(x);
                data << x;
                {
                    ios::imstream inp(s);
                    const uint16_t j = inp.read_upack<uint16_t>();
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
                fp.emit_upack<uint64_t>(x);
                op.emit_upack<uint64_t>(x);
                data << x;
                {
                    ios::imstream inp(s);
                    const uint32_t j = inp.read_upack<uint32_t>();
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
                fp.emit_upack<uint64_t>(x);
                op.emit_upack<uint64_t>(x);
                data << x;
                {
                    ios::imstream inp(s);
                    const uint64_t j = inp.read_upack<uint64_t>();
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
                const uint64_t loaded = fp.read_upack<uint64_t>();
                Y_ASSERT(loaded==data[i]);
            }
        }
    }

}
Y_UTEST_DONE()


