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

#include "y/associative/hash/map.hpp"

#include "y/program.hpp"

using namespace upsylon;

typedef core::cpp_node_of<string>   argument;
typedef core::list_of_cpp<argument> arguments;

typedef arc_ptr<hashing::function>  hproc;
typedef hash_map<string,hproc>      hpmap;

#define DECL(NAME) do { \
hproc  p = new hashing::NAME();\
if(!H) H = & *p;\
const string k = p->name();          \
if(!db.insert(k,p)) throw exception("%s: multiple '%s'",program,*k);\
} while(false)

#define ALGO(NAME) do { if(#NAME==algo) { H = new hashing::NAME(); delete args.pop_front(); goto READY; } } while(false)

Y_PROGRAM_START()
{
    hashing::function *H = 0;
    hpmap db;
    DECL(md5);
    DECL(sha1);
    DECL(crc32);
    DECL(md4);
    DECL(md2);
    DECL(rmd128);
    DECL(rmd160);
    DECL(sha224);
    DECL(sha256);
    DECL(sha384);
    DECL(sha512);
    assert(H);
    
    if(argc>1&&0==strcmp("-h", argv[1]) )
    {
        std::cout << "usage:" << std::endl;
        std::cout << program << '[';
        size_t n=db.size();
        for(hpmap::iterator it=db.begin();it!=db.end();++it)
        {
            std::cout << it.get().key();
            if(--n>0) std::cout << '|';
        }
        std::cout << ']' << " [files]" << std::endl;
        return 0;
    }
    
    arguments           args;
    for(int i=1;i<argc;++i)
    {
        const string a = argv[i];
        args.push_back( new argument(a) );
    }
    //--------------------------------------------------------------------------
    //
    // change algorithm ?
    //
    //--------------------------------------------------------------------------
    
    if(args.size>0)
    {
        const string algo = string_convert::to_lower( **args.head );
        hproc       *ppH  = db.search(algo);
        if(ppH)
        {
            H = & **ppH;
            delete args.pop_front();
        }
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


