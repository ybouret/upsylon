
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/fs/disk/istream.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/string.hpp"
#include "y/os/wtime.hpp"

using namespace upsylon;

Y_UTEST(istreams)
{

    {
        const string  data = "Hello, World";
        ios::imstream fp(data);
        char C=0;
        while( fp.query(C) )
        {
            fp.store(C);
            Y_ASSERT(fp.query(C));
            std::cerr << C;
        }
        std::cerr << std::endl;
    }

    if(argc>1)
    {


        const string fn = argv[1];
        double duration = 0;
        if(fn != Y_STDIN)
        {
            string line(1024,as_capacity,false);
            double std_speed = 0;
            Y_TIMINGS(std_speed,duration,
                      ios::icstream fp(fn);
                      while( fp.gets(line) )
                      ;
                      );

            line.clear();

            std::cerr << "std speed: " << std_speed << std::endl;

            double raw_speed = 0;
            ios::shared_disk_buffer buf = new ios::disk_buffer<>(BUFSIZ);
            Y_TIMINGS(raw_speed,duration,
                      ios::readable_disk_file raw(fn);
                      ios::disk_istream       fp(raw,buf);
                      while( fp.gets(line) )
                      ;
                      );

            std::cerr << "raw speed: " << raw_speed << std::endl;

        }

    }


}
Y_UTEST_DONE()
