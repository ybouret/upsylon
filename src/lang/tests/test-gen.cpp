
#include "y/lang/dynamo.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(gen)
{
    //vfs &fs = local_fs::instance();
    if(argc>1)
    {
        const string      filename = argv[1];
        Dynamo            compiler(filename,Dynamo::SourceFile,true);
        std::cerr << "Dynamo for '" << compiler->name << "'" << std::endl;
        {
            const string savename = *(compiler->name)+".bin";
            const string bin      = Dynamo::ToBinary(filename);
            {
                ios::ocstream fp(savename); fp << bin;
            }
            {
                std::cerr << "Reloading from Binary File" << std::endl;
                Dynamo compiler2(savename,Dynamo::BinaryFile,false);
            }
            {
                std::cerr << "Reloading from Binary Data" << std::endl;
                Dynamo compiler3(*savename,*bin,bin.size(),false);
            }
        }

        if(argc>2)
        {
            const string src = argv[2];
            Module      *m   = 0;
            if(src == "run")
            {
                m = Module:: OpenSTDIN();
            }
            else
            {
                m = Module:: OpenFile(src);
            }
            compiler.compile(m);
        }
    }
}
Y_UTEST_DONE()


