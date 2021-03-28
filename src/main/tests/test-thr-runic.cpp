#include "y/concurrent/loop/runic.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    struct ops
    {
        size_t length;
        
        static inline void run(const concurrent::context &cntx,
                               memory::shack             &data,
                               void                      *args,
                               lockable                  &sync) throw()
        {
            assert(args);
            ops &self = *static_cast<ops *>(args);
           
            double sum = 0;
            size_t offset = 1;
            size_t length = self.length;
            cntx.split(length,offset);
            {
                Y_LOCK(sync);
                std::cerr << "@" << cntx.label << " : " << offset << " +" << length << std::endl;
            }
            while(length-- > 0)
            {
                sum += 1.0 / square_of(offset++);
            }
            data.as<double>() = sum;
        }
        
    };
    
}

#include "y/string/convert.hpp"

Y_UTEST(thr_runic)
{
    ops todo = { 1000000 };
    if(argc>1)
    {
        todo.length = string_convert::to<size_t>(argv[1],"length");
    }
    concurrent::runic engine(new concurrent::simt());
    engine.make<double>();
    
    engine(ops::run,&todo);
    std::cerr << "sum: " << sqrt(6.0*engine.sum<double>()) << std::endl;
    
}
Y_UTEST_DONE()

