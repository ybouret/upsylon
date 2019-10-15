#include "y/ios/tools/posix-command.hpp"
#include "y/string.hpp"
#include "y/sequence/list.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(cmd)
{
    if(argc>1)
    {
        string cmd = argv[1];
        for(int i=2;i<argc;++i)
        {
            cmd << ' ' << argv[i];
        }
        std::cerr << "?[" << cmd << "]" << std::endl;
        list<string> lines;
        ios::posix_command::query(lines,cmd);
        for(size_t i=1;i<=lines.size();++i)
        {
            std::cerr << lines[i] << std::endl;
        }
    }
}
Y_UTEST_DONE();
