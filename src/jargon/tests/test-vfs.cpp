#include "y/jargon/pattern/vfs.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local/fs.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(vfs)
{
    if(argc>1)
    {
        VFS_Matcher matcher = argv[1];
        matcher->graphViz("vfs-matcher.dot");
        if(argc>2)
        {
            vfs                   &fs   = local_fs::instance();
            const size_t           count = matcher.matchExtensions(NULL, fs, argv[2]);
            list<string>           matching(count,as_capacity);
            Y_CHECK( count==matcher.matchExtensions(&matching,fs,argv[2]) );
            std::cerr << matching << std::endl;
            std::cerr << "count=" << count << std::endl;
            

        }
    }
}
Y_UTEST_DONE()
