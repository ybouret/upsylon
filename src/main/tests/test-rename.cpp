#include "y/fs/local/fs.hpp"
#include "y/utest/run.hpp"
#include "y/hashing/sha1.hpp"
#include "y/ios/ocstream.hpp"
#include "y/fs/disk/file.hpp"

using namespace upsylon;

Y_UTEST(rename)
{
    vfs         &fs       = local_fs::instance();
    const string old_path = fs.get_temporary_name();
    const string new_path = fs.get_temporary_name();

    {
        ios::ocstream fp(old_path);
        for(size_t i=10+alea.leq(1000);i>0;--i)
        {
            fp << alea.range<char>('a','z');
        }
    }

    hashing::sha1 h;
    const digest  old_digest = ios::readable_disk_file::md(h,old_path);
    fs.rename_path(old_path,new_path);
    Y_CHECK(false==fs.is_reg(old_path));
    Y_CHECK(true==fs.is_reg(new_path));
    const digest new_digest = ios::readable_disk_file::md(h,new_path);
    Y_CHECK(new_digest==old_digest);
    fs.try_remove_file(new_path);

}
Y_UTEST_DONE()

