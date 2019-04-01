#include "y/program.hpp"
#include "y/ios/rc/io.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;

static inline
void add_rc(ios::rc::writer &rc,
            const string    &parent,
            const string    &arg )
{
    vfs &fs = local_fs::instance();

    vfs::entry ep(arg,fs);

    if(ep.is_regular())
    {
        // add the file
        
    }

}

Y_PROGRAM_START()
{
    if(argc<=1)
    {
        return 0;
    }

    const string filename = argv[1];
    {

        ios::rc::writer rc(filename,true);
        string parent = "";
        for(int iarg=2;iarg<argc;++iarg)
        {
            const string arg = argv[iarg];
            add_rc(rc,parent,arg);
        }
    }



}
Y_PROGRAM_END()

