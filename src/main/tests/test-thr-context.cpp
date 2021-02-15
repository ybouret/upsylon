#include "y/concurrent/context.hpp"
#include "y/utest/run.hpp"

#include "y/type/ints-utils.hpp"

using namespace upsylon;

namespace
{
    static inline void displayCtx(const concurrent::context &ctx )
    {
        std::cerr << "@" << ctx.size << "." << ctx.rank << "\t: " << ctx.label << std::endl;
    }
}

Y_UTEST(thr_context)
{



    {
        const concurrent::context ctx;
        displayCtx(ctx);
    }

    for(size_t size=1;size<=16;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            const concurrent::context ctx(size,rank);
            std::cerr << "\t";
            displayCtx(ctx);
        }
    }


    std::cerr << "sizeof(context)=" << sizeof(concurrent::context) << std::endl;

    for(int i=1;i<argc;++i)
    {
        const int s = atoi(argv[i]);
        std::cerr << "signed   digits for " << s << " : " << core::count_digits::base10(s,int2type<true>()) << std::endl;
        if(s>=0)
        {
            const unsigned u = unsigned(s);
            std::cerr << "unsigned digits for " << u << " : " << core::count_digits::base10(u,int2type<false>()) << std::endl;
        }
        std::cerr << "auto     digits for " << s << " : " << core::count_digits::base10(s) << std::endl;


    }
}
Y_UTEST_DONE()

