#include "y/fs/disk/istream.hpp"
#include "y/fs/disk/ostream.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/string.hpp"

#include <cstdio>

using namespace upsylon;

Y_UTEST(disk_std)
{
    ios::shared_disk_buffer ish = new ios::disk_buffer<>(BUFSIZ);
    ios::shared_disk_buffer osh = new ios::disk_buffer<memory::pooled>(32);
    ios::shared_disk_buffer esh = new ios::disk_buffer<memory::pooled>(64);

    ios::readable_disk_file src( ios::cstdin );
    ios::disk_istream       inp(src,ish);

    ios::writable_disk_file tgtout(ios::cstdout);
    ios::disk_ostream       out(tgtout,osh);

    ios::writable_disk_file tgterr(ios::cstderr);
    ios::disk_ostream       err(tgterr,esh);

    string line;
    while( inp.gets(line) )
    {
        std::cerr << line << std::endl;
        out << line << '\n';
        err << line << '\n';
    }
    out.flush();
    err.flush();
}
Y_UTEST_DONE()
