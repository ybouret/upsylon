#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/lang/pattern/matching.hpp"
#include "y/program.hpp"

using namespace upsylon;

Y_PROGRAM_START()
{
    if( argc > 1 )
    {
        const string      expr = argv[1];
        Lang::MatchString keep = expr;      keep.motif->save_to( "grep.bin" );
        const string      fileName = (argc > 2) ? argv[2] : Y_STDIN;
        ios::icstream     input(  fileName     );
        ios::ocstream     output( ios::cstdout );
        string            line;
        while( input.gets(line) )
        {
            if( keep(line) )
            {
                output << line << '\n';
            }
        }
    }
}
Y_PROGRAM_END()
