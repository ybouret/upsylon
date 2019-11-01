#include "y/program.hpp"
#include "y/string/convert.hpp"
#include "y/os/hw.hpp"
#include "y/ios/ocstream.hpp"
#include <cstdlib>

using namespace upsylon;

Y_PROGRAM_START()
{
    // get parameters
    if(argc<=3)
    {
        throw exception("usage: %s output pi_executable nmax [sampling_time]",program);
    }
    const string dataName = argv[1];
    const string progName = argv[2];
    size_t       nmax     = string_convert::to<size_t>(argv[3],"nmax");
    if(nmax<=0)
    {
        nmax = hardware::nprocs();
    }
    double sampling_time = 0;
    if(argc>4)
    {
        sampling_time = string_convert::to<double>( argv[4], "sampling_time" );
    }


    // initialize
    std::cerr << "cleaning: [" << dataName << "]" << std::endl;
    ios::ocstream::overwrite(dataName);

    // loop
    for(size_t n=1;n<=nmax;++n)
    {
        const string cmd = "nice -n 0 " + progName + vformat(" %u %.3g >> ", unsigned(n), sampling_time ) + dataName;
        std::cerr << "running:  [" << cmd << "]" << std::endl;
        if( EXIT_SUCCESS != system( *cmd ) )
        {
            throw exception("system() failure");
        }
    }


}
Y_PROGRAM_END()
