#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
 #include "y/jargon/pattern/matcher.hpp"
#include "y/program.hpp"

using namespace upsylon;
using namespace Jargon;

Y_PROGRAM_START()
{
    if( argc > 1 )
    {
        Token        token;
        Matcher      keep = argv[1];
        keep->save_to( "grep.bin" );
        const string      fileName = (argc > 2) ? argv[2] : Y_STDIN;
        ios::icstream     input(  fileName     );
        ios::ocstream     output( ios::cstdout );
        string            line;
        while( input.gets(line) )
        {
            if( keep.matches_somehow(line) )
            {
                output << line << '\n';
            }
        }
    }
}
Y_PROGRAM_END()
