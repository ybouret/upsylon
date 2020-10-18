#include "y/rtld/dll.hpp"
#include "y/rtld/rtld.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

typedef double (Y_DLL_API *func)(double);

Y_UTEST(dll)
{
    rtld &libs = rtld::instance();
    
    if(argc>1)
    {
        const string soname = argv[1];
        dll so( soname );
        std::cerr << "system handle@" << so.get_system_handle() << std::endl;
        {
            dll so2( soname );
            std::cerr << "system handle@" << so2.get_system_handle() << std::endl;
        }
        func Sin = so.hook<func>("Sin");
        if(Sin)
        {
            std::cerr << "Found Sin" << std::endl;
            for(double x=0;x<3;x+=0.2)
            {
                std::cerr << "Sin(" <<  x << ")=" << x << std::endl;
            }
        }
        else
        {
            std::cerr << "Missing Sin" << std::endl;
        }

        int32_t *Data = so.ptr<int32_t>("Data");
        if(Data)
        {
            std::cerr << "Found Data=" << *Data << std::endl;
        }
        else
        {
            std::cerr << "Data not found" << std::endl;
        }

    }
    
    std::cerr << "linked=" << libs.linked() << std::endl;
}
Y_UTEST_DONE()


