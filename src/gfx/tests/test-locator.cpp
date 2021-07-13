#include "y/gfx/edges/locator.hpp"
#include "y/utest/run.hpp"

#include "y/gfx/image/io.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/filters/db.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(locator)
{
    Y_USE_IMG();
    engine                eng = new concurrent::simt();
    //engine                eng = new concurrent::solo();
    filters_db           &fdb = filters_db::instance();
    shared_knots          kdb = new knots();
    auto_ptr<edges::blur> blr = NULL;
    blobs                 B;
    size_to_rgba          sz2c;

    if(argc>1)
    {
        const pixmap<rgba> source = IMG.load<rgba>(argv[1]);
        IMG.save(source,"source.png");
        string label = "full";
        if(argc>2)
        {
            blr   = new edges::blur(5,5,string_convert::to<float>(argv[2],"sigma"));
            label = "fuzz";
        }

        broker apply(eng,source);

        for(filters_db::iterator it=fdb.begin();it!=fdb.end();++it)
        {
            const string &id = it->key();
            std::cerr << "using [" << id << "]" << std::endl;
            edges::locator E(id,source.w,source.h,kdb);
            const size_t num = E.compat_find(B,source,apply,blr.content(),1);
            std::cerr << "\t#edges=" << num << std::endl;
            const string filename = id + '-' + label + ".png";
            IMG.save(E,filename,NULL,sz2c);
        }
    }

}
Y_UTEST_DONE()

