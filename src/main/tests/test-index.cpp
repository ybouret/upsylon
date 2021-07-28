#include "y/sort/index.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void doIndx()
    {
        vector<T>      data;
        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            const T tmp = support::get<T>();
            data.push_back(tmp);
        }

        vector<size_t> indx(data.size(),0);
        indexing::make(indx,comparison::increasing<T>,data);
        vector<T>      outp(data);
        indexing::rank(outp,data,indx);
        std::cerr << "data: " << data << std::endl;
        std::cerr << "indx: " << indx << std::endl;
        std::cerr << "outp: " << data << std::endl;

        for(size_t i=1;i<outp.size();++i)
        {
            Y_ASSERT(outp[i]<=outp[i+1]);
        }
        Y_CHECK( say("sorted...") );
        std::cerr << std::endl;

    }
}

Y_UTEST(index)
{
    doIndx<uint16_t>();
    doIndx<string>();
    doIndx<apq>();

}
Y_UTEST_DONE()

