#include "y/hashing/sha1.hpp"
#include "y/hashing/md5.hpp"
#include "y/string/display.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local/fs.hpp"
#include "y/fs/disk/file.hpp"

using namespace upsylon;

Y_UTEST(md)
{

    hashing::md5 h_md5;
    
    for(int i=1;i<argc;++i)
    {
        const string fileName = argv[i];
        const string baseName = vfs::get_base_name(fileName);
        string_display::align(std::cerr,baseName,32);
        {
            const digest d = ios::disk_file::md(h_md5, fileName);
            std::cerr << " " << h_md5.name() << ":" << d;
        }
        std::cerr << std::endl;
    }
}
Y_UTEST_DONE()
