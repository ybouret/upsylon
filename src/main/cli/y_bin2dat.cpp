#include "y/ios/tools/bin2dat.hpp"
#include "y/program.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

#define WIDTH 16

Y_PROGRAM_START()
{
    if(argc<=2) throw exception("usage: %s input output", program);

    //const string inpFileName = argv[1];
    //const string outFileName = argv[2];

    vector<char> content(16386,as_capacity);
    {
        ios::icstream fp( argv[1] );
        char          C = 0;
        while( fp.query(C) )
        {
            content.push_back(C);
        }
    }

    {
        ios::ocstream fp( argv[2] );
        const size_t n = content.size();
        if(n>0)
        {
            ios::bin2dat  b2d(WIDTH);
            for(size_t i=1;i<n;++i)
            {
                b2d.write(fp,content[i],false);
            }
            b2d.write(fp,content[n],true);
        }
    }
    

}
Y_PROGRAM_END()

