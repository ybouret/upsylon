#include "y/associative/suffix-store.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(store)
{
    if(true)
    {
        Y_UTEST_SIZEOF( suffix_store<uint8_t>::node_type  );
        Y_UTEST_SIZEOF( suffix_store<uint16_t>::node_type );
        Y_UTEST_SIZEOF( suffix_store<uint32_t>::node_type );
        Y_UTEST_SIZEOF( suffix_store<uint64_t>::node_type );
    }

    suffix_store<char>     s8;
    suffix_store<uint16_t> s16;
    suffix_store<uint32_t> s32;
    suffix_store<uint64_t> s64;

    vector<string>     keys;
    size_t             total=0;
    if(argc>1)
    {
        const string  fileName = argv[1];
        ios::icstream fp( fileName);
        string        line;

        while(fp.gets(line))
        {
            if(s8.insert( *line, line.size() ))
            {
                keys << line;
                std::cerr << "+ '" << line << "'" << std::endl;
                total += line.size();
                Y_ASSERT(s16.insert( *line, line.size() ));
                Y_ASSERT(s32.insert( *line, line.size() ));
                Y_ASSERT(s64.insert( *line, line.size() ));

            }
        }
    }


    std::cerr << "#nodes  = " << s8.nodes  << " -> " <<  s8.nodes * sizeof(suffix_store<uint8_t>::node_type) << std::endl;
    std::cerr << "        = " << s16.nodes << " -> " << s16.nodes * sizeof(suffix_store<uint16_t>::node_type) <<std::endl;
    std::cerr << "        = " << s32.nodes << " -> " << s32.nodes * sizeof(suffix_store<uint32_t>::node_type) <<std::endl;
    std::cerr << "        = " << s64.nodes << " -> " << s64.nodes * sizeof(suffix_store<uint64_t>::node_type) <<std::endl;

    std::cerr << "#bytes  = " << total    << std::endl;
    
}
Y_UTEST_DONE()

