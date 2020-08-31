
#include "y/jive/pattern/matching.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/icstream.hpp"
#include "y/fs/vfs.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_match)
{

    if(argc>1)
    {
        Matching match = argv[1];

        if( argc>2 )
        {
            const string  fileName = argv[2];
            const string  baseName = vfs::get_base_name(fileName);
            ios::icstream fp(fileName);
            string        line;
            unsigned      iline=1;
            while( (std::cerr << "> ").flush(),fp.gets(line) )
            {
                const string dataName = baseName + vformat(":%u: ",iline);
                Source       source( Module::OpenData(dataName,line) );
                ++iline;
            }
        }

    }

}
Y_UTEST_DONE()
