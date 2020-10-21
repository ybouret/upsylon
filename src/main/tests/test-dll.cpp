#include "y/rtld/dll.hpp"
#include "y/rtld.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

typedef double (Y_DLL_API *func)(double);

Y_UTEST(dll)
{
    std::cerr << "<dll>" << std::endl;
    if(argc>1)
    {
        const string soname = argv[1];
        dll  so( soname );
        func Sin = dll::hook<func>::load(so,"Sin");
        if(Sin)
        {
            std::cerr << "\tFound Sin" << std::endl;
            for(double x=0;x<3;x+=0.2)
            {
                std::cerr << "\t\tSin(" <<  x << ")=" << Sin(x) << std::endl;
            }
        }
        else
        {
            std::cerr << "\tMissing Sin" << std::endl;
        }
        int32_t *Data = dll::data<int32_t>::load(so,"Data");
        if(Data)
        {
            std::cerr << "\tFound Data=" << *Data << std::endl;
        }
        else
        {
            std::cerr << "\tData not found" << std::endl;
        }
        
    }
    std::cerr << "<dll/>" << std::endl;

}
Y_UTEST_DONE()


