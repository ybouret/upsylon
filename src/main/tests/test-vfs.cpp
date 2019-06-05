#include "y/fs/local/fs.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{
    static inline void display( const string &str, const size_t extend )
    {
        std::cerr << str; for(size_t i=str.size();i<extend;++i) std::cerr << ' ';
    }

}
Y_UTEST(vfs)
{
    vfs & fs = local_fs::instance();

    if(argc>1)
    {
        std::cerr << "scanning '" << argv[1] << "'" << std::endl;
        auto_ptr<vfs::scanner> scan = fs.new_scanner(argv[1]);
        list<vfs::entry> entries;
        size_t           max_plen = 0;
        size_t           max_nlen = 0;
        for(const vfs::entry *ep = scan->next(); ep; ep=scan->next() )
        {
            entries.push_back( *ep );
            const size_t plen = ep->path.size();
            if(plen>max_plen)
            {
                max_plen = plen;
            }
            const size_t nlen = length_of(ep->base_name);
            if(nlen>max_nlen)
            {
                max_nlen = nlen;
            }

#if 0
            std::cerr << ep->path << " | " << ep->base_name;
            std::cerr << " | link=" << ep->link;
            std::cerr << " | attr=" << ep->attr << "/" << ep->attr2text();
            if(ep->extension) { std::cerr << " [" << ep->extension << "]"; }
            std::cerr << std::endl;
#endif
        }

        std::cerr << "#entries=" << entries.size() << std::endl;
        // format output
        for(list<vfs::entry>::iterator ep=entries.begin(); ep != entries.end(); ++ep )
        {
            display(ep->path,max_plen); std:: cerr << '|';
            display(ep->base_name,max_nlen);
            std::cerr << " | link=" << ep->link;
            std::cerr << " | attr=" << ep->attr << "/" << ep->attr2text();
            if(ep->extension) { std::cerr << " [" << ep->extension << "]"; }
            std::cerr << std::endl;
        }
    }
}
Y_UTEST_DONE()


