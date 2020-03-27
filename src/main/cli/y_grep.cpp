#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/jargon/pattern.hpp"
#include "y/jargon/pattern/regexp.hpp"
#include "y/program.hpp"

using namespace upsylon;
using namespace Jargon;

Y_PROGRAM_START()
{
    if( argc > 1 )
    {
        Cache cache;
        Token token(cache);
        const string expr = argv[1];
        Motif        keep = RegularExpression::Compile(expr);
        keep->save_to( "grep.bin" );
        const string      fileName = (argc > 2) ? argv[2] : Y_STDIN;
        ios::icstream     input(  fileName     );
        ios::ocstream     output( ios::cstdout );
        string            line;
        while( input.gets(line) )
        {
            if( keep->matches_partly(token,line) )
            {
                output << line << '\n';
            }
        }
    }
}
Y_PROGRAM_END()
