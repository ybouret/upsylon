#include "y/hashing/sha1.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/crc32.hpp"
#include "y/hashing/md4.hpp"
#include "y/hashing/md2.hpp"
#include "y/hashing/rmd128.hpp"
#include "y/hashing/rmd160.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"

#include "y/string/convert.hpp"

#include "y/fs/local/fs.hpp"
#include "y/fs/disk/file.hpp"
#include "y/ios/icstream.hpp"
#include "y/core/node.hpp"
#include "y/program.hpp"

using namespace upsylon;

typedef core::cpp_node_of<string>   argument;
typedef core::list_of_cpp<argument> arguments;

#define ALGO(NAME) do { if(#NAME==algo) { H = new hashing::NAME(); delete args.pop_front(); goto READY; } } while(false)

Y_PROGRAM_START()
{
    auto_ptr<hashing::function> H = NULL;
    arguments                   args;

    //--------------------------------------------------------------------------
    //
    // change algorithm ?
    //
    //--------------------------------------------------------------------------
    {
        for(int i=1;i<argc;++i)
        {
            const string a = argv[i];
            args.push_back( new argument(a) );
        }
        if(args.size>0)
        {
            const string algo = string_convert::to_lower( **args.head );
            ALGO(md5);
            ALGO(sha1);
            ALGO(crc32);
            ALGO(md4);
            ALGO(md2);
            ALGO(rmd128);
            ALGO(rmd160);
            ALGO(sha224);
            ALGO(sha256);
            ALGO(sha384);
            ALGO(sha512);
        }
        assert( H.is_empty() );
        H = new hashing::md5();

    READY:
        assert(H.is_valid());
    }

    //--------------------------------------------------------------------------
    //
    // check args
    //
    //--------------------------------------------------------------------------
    if(args.size<=0)
    {
        ios::icstream fp(ios::cstdin);
        H->set();
        char C = 0;
        while(fp.query(C)) H->run(&C,1);
        const digest md = H->md();
        std::cout << md << ' ' << H->name() << "(stdin)" << std::endl;
    }
    else
    {
        while(args.size)
        {
            const string &file_id = **args.head;
            const digest  file_md = ios::disk_file::md(*H,file_id);
            std::cout << file_md << ' ' << H->name() << '(' << file_id << ')' << std::endl;
            delete args.pop_front();
        }
    }
}
Y_PROGRAM_END()


