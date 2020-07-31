#include "y/sequence/slots.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "support.hpp"

using namespace upsylon;

Y_UTEST(slots)
{
    {
        slots<double> ds(10);
        while(ds.size()<ds.capacity())
        {
            //ds.push();
            ds.build<int>(alea.full<int>());
        }
        for(size_t i=0;i<ds.size();++i)
        {
            std::cerr << "ds[" << i << "]=" << ds[i] << std::endl;
        }
    }

    {
        slots<string,memory::pooled> ts(17);
        for(size_t i=0;i<ts.capacity();++i)
        {
            if(alea.choice())
            {
                ts.build();
            }
            else
            {
                if(alea.choice())
                {
                    ts.build<const char*>( "hello" );
                }
                else
                {
                    const string s = support::get<string>();
                    ts.build<string>(s);
                }
            }
        }

        for(size_t i=0;i<ts.size();++i)
        {
            std::cerr << "ts[" << i << "]=" << ts[i] << std::endl;
        }

    }
}
Y_UTEST_DONE()


