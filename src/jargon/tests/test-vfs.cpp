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

#if 0
            auto_ptr<vfs::scanner> scan = fs.scan(argv[2]);
            for(const vfs::entry *ep = scan->next(); ep; ep=scan->next() )
            {
                if(!ep->is_regular()) continue;
                std::cerr << ep->path << std::endl;
                if(matcher.matchExtensionOf(*ep))
                {
                    matching.push_back(ep->path);
                }
            }
            std::cerr << "matching: " << matching << std::endl;
#endif

        }
    }
}
Y_UTEST_DONE()
