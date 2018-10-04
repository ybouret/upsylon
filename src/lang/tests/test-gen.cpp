
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
            const string bin      = Dynamo::Compile(filename);
            {
                ios::ocstream fp(savename); fp << bin;
            }
            Dynamo compiler2(savename,Dynamo::BinaryFile,false);
            Dynamo compiler3(*savename,*bin,bin.size(),false);
        }
    }
}
Y_UTEST_DONE()


