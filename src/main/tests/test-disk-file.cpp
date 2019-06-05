#include "y/fs/disk/istream.hpp"
#include "y/fs/disk/ostream.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"

#include <cstdio>

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




    ios::shared_disk_buffer ish = new ios::disk_buffer<>(BUFSIZ);
    ios::shared_disk_buffer osh = new ios::disk_buffer<memory::pooled>(32);

    {
        ios::readable_disk_file src(id);
        ios::disk_istream       fp(src,ish,4);
        string line;
        char C=0;
        while(fp.query(C))
        {
            std::cerr << C;
            line += C;
        }
        std::cerr << std::endl;
        Y_CHECK("Hell" == line);
    }

    if(argc>1)
    {
        ios::readable_disk_file src( argv[1] );
        ios::disk_istream       inp(src,ish);
        const string            outname = "wdf.bin";
        ios::writable_disk_file tgt(outname,false);
        ios::disk_ostream       out(tgt,osh );
        string line;
        while( inp.gets(line) )
        {
            std::cerr << line << std::endl;
            out << line << '\n';
        }
        out.flush();
    }
}
Y_UTEST_DONE()


