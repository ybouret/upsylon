#include "y/hashing/factory.hpp"

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



Y_PROGRAM_START()
{
    hashing::factory &hfac = hashing::factory::instance();
    
    if(argc>1&&0==strcmp("-h", argv[1]) )
    {
        std::cout << "usage:" << std::endl;
        std::cout << program << ' ' << '[';
        size_t n=hfac.size();
        for(hashing::factory::iterator it=hfac.begin();it!=hfac.end();++it)
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
    auto_ptr<hashing::function> H = NULL;
    if(args.size>0)
    {
        const string algo = string_convert::to_lower( **args.head );
        if( hfac.search(algo) )
        {
            H = hfac( algo );
            delete args.pop_front();
        }
    }
    if(H.is_empty()) H = hfac("md5");
    
    
    //--------------------------------------------------------------------------
    //
    // check args
    //
    //--------------------------------------------------------------------------
    if(args.size<=0)
    {
        const digest md = ios::readable_disk_file::md(*H,ios::cstdin);
        std::cout << md << ' ' << H->name() << "(stdin)" << std::endl;
    }
    else
    {
        while(args.size)
        {
            const string &file_id = **args.head;
            const digest  file_md = ios::readable_disk_file::md(*H,file_id);
            std::cout << file_md << ' ' << H->name() << '(' << file_id << ')' << std::endl;
            delete args.pop_front();
        }
    }
}
Y_PROGRAM_END()


