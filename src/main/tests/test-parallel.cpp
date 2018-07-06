#include "y/parallel.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(parallel)
{


    for(size_t length=0;length<=10;++length)
    {
        std::cerr << "splitting length=" << length << std::endl;
        for(size_t cores=1;cores<=8;++cores)
        {
            std::cerr << "\t#core=" << cores << ":";
            size_t sum = 0;
            for(size_t rank=0;rank<cores;++rank)
            {
                size_t todo = length;
                size_t init = 1;
                parallel par(cores,rank);
                par.split(todo,init);
                sum += todo;
                if(todo>0)
                {
                    std::cerr << '[' << init << '-' << init+todo-1 << ']';
                }
                else
                {
                    std::cerr << "[#]";
                }
            }
            std::cerr << " => ";Y_CHECK(sum==length);
        }
    }

}
Y_UTEST_DONE()

