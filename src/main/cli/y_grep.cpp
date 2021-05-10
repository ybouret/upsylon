#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/jive/pattern/matching.hpp"
#include "y/program.hpp"

using namespace upsylon;
using namespace Jive;

Y_PROGRAM_START()
{
    if( argc > 1 )
    {
        Token        token;
        Matching     keep = argv[1];
        keep.motif->save_to(  "grep.bin" );
        keep.motif->graphViz( "grep.dot" );
        const string      fileName = (argc > 2) ? argv[2] : Y_STDIN;
        ios::icstream     input(  fileName     );
        ios::ocstream     output( ios::cstdout );
        string            line;
        while( input.gets(line) )
        {
            const Token *tkn = keep.isFoundIn(line);
            if( tkn )
            {
                output << line << " => [" << *tkn << "]\n";
            }
        }
    }
}
Y_PROGRAM_END()
