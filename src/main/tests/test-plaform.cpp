#include "y/utest/run.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"

using namespace upsylon;

Y_UTEST(platform)
{
    std::cerr << "sizeof(exception)=" << sizeof(upsylon::exception) << std::endl;
    Y_CHECK(sizeof(int8_t)==1);
    Y_CHECK(sizeof(int16_t)==2);
    Y_CHECK(sizeof(int32_t)==4);
    Y_CHECK(sizeof(int64_t)==8);

    Y_CHECK(sizeof(uint8_t)==1);
    Y_CHECK(sizeof(uint16_t)==2);
    Y_CHECK(sizeof(uint32_t)==4);
    Y_CHECK(sizeof(uint64_t)==8);

    {
        const libc::exception e( 1, "Libc Error 1" );
        std::cerr << "Libc Error " << e.code() << " = " << e.what() << ", " << e.when() << std::endl;
    }

    {
        const win32::exception e( 1, "Win32 Error 1" );
        std::cerr << "Win  Error " << e.code() << " = " << e.what() << ", " << e.when() << std::endl;
    }

    {
        const os_exception e( error_invalid_data, "OS Invalid Data");
        std::cerr << "O/S  Error " << e.code() << " = " << e.what() << ", " << e.when() << std::endl;

    }
}
Y_UTEST_DONE()
