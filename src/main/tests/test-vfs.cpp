#include "y/fs/local.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(vfs)
{
    vfs & fs = local_fs::instance();

    if(argc>1)
    {
        std::cerr << "scanning '" << argv[1] << "'" << std::endl;
        auto_ptr<vfs::scanner> scan = fs.new_scanner(argv[1]);
        for(const vfs::entry *ep = scan->next(); ep; ep=scan->next() )
        {
            std::cerr << ep->path << " | " << ep->base_name;
            if(ep->extension) { std::cerr << " [" << ep->extension << "]"; }
            std::cerr << " | link=" << ep->link;
            std::cerr << " | attr=" << ep->attr;
            std::cerr << std::endl;
        }
    }
}
Y_UTEST_DONE()


