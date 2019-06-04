#include "y/fs/disk-istream.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(disk_file)
{
    std::cerr << "sizeof(on_disk)   = " << sizeof(ios::on_disk)   << std::endl;
    std::cerr << "sizeof(disk_file) = " << sizeof(ios::disk_file) << std::endl;

    const char  *id = "toto.bin";
    const string fn = id;

    {
        ios::writable_disk_file fp(id,false);
        fp.put("Hello", 5);

        {
            ios::writable_disk_file fp1(fp);
            fp.put(", World", 7);
            std::cerr << "length=" << fp1->length() << std::endl;
        }

        std::cerr << "length=" << fp->length() << std::endl;
        fp.put("!",1);
        std::cerr << "length=" << fp->length() << std::endl;
    }

    {
        ios::writable_disk_file fp(fn,true);
        fp.put("...",3);
        std::cerr << "length=" << fp->length() << std::endl;
    }

    char buff[16];
    {
        ios::readable_disk_file fp(id);
        fp->unwind();
        fp->seek(-3,ios::from_cur);
        memset(buff,0,sizeof(buff));
        Y_CHECK(3==fp.get(buff,3));
        std::cerr << "<" << buff << ">" << std::endl;

        {
            ios::readable_disk_file fp1(fp);
            fp1->rewind();
            memset(buff,0,sizeof(buff));
            Y_CHECK(12==fp1.get(buff,12));
            std::cerr << "<" << buff << ">" << std::endl;
        }

        memset(buff,0,sizeof(buff));
        Y_CHECK(1==fp.get(buff,1));
        std::cerr << "<" << buff << ">" << std::endl;
    }


    ios::shared_disk_buffer sh = new ios::disk_buffer<>(100);
    if(argc>1)
    {
        ios::readable_disk_file src( argv[1] );
        ios::disk_istream       inp(src,sh);
        string line;
        while( inp.gets(line) )
        {
            std::cerr << line << std::endl;
        }
    }
}
Y_UTEST_DONE()

