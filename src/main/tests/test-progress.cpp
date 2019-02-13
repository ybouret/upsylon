#include "y/os/progress.hpp"
#include "y/utest/run.hpp"
#include "y/os/wtime.hpp"
#include <cstdio>

using namespace upsylon;

Y_UTEST(progress)
{
    std::cerr << "-- format..." << std::endl;
    char fmt[progress::format_size];
    for(size_t iter=0;iter<10;++iter)
    {
        const double tmx = alea.to<double>() * (2*progress::max_seconds);
        progress::format(fmt,tmx);
        std::cerr.flush();
        fflush(stderr);
        fprintf(stderr,"tmx=%15.3f => ",tmx);
        fflush(stderr);
        std::cerr << "'"  << fmt << "'" << std::endl;
    }

    std::cerr << "-- progres..." << std::endl;
    progress bar;
    wtime    chrono;

    bar.start();
    const size_t iter_max = 1 + alea.leq(500);
    for(size_t i=1;i<=iter_max;++i)
    {
        chrono.sleep( 0.01 * alea.to<double>() );
        bar.update(i,iter_max);
        //std::cerr << "Done: " << bar.done << std::endl;
        //std::cerr << "Left: " << bar.left << std::endl;
        bar.display(std::cerr) << '\r';
    }
    std::cerr << std::endl;

}
Y_UTEST_DONE()

