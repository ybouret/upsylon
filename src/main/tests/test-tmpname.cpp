#include "y/string/temporary-name.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(tmpname)
{
    vfs &fs = local_fs::instance();
    for(size_t i=0;i<32;++i)
    {
        const string tmp = temporary_name( alea.leq(16) );
        std::cerr << tmp;
        std::cerr << '+';
        ios::ocstream::overwrite(tmp);
        std::cerr << '-';
        fs.remove_file(tmp);
        std::cerr << std::endl;
    }
}
Y_UTEST_DONE()

