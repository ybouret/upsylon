#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/type/aliasing.hpp"

using namespace upsylon;

Y_PROGRAM_START()
{
    if(argc<=2 ) throw exception("usage: %s [input|-] [output[-]", program);

    const string inputFile  = argv[1];
    const string outputFile = argv[2];

    auto_ptr<ios::istream> input  = 0;
    if("-"==inputFile)
    {
        input                  = new ios::icstream( ios::cstdin );
        aliasing::_(inputFile) = "STDIN";
    }
    else
    {
        input = new ios::icstream( inputFile );
    }

    auto_ptr<ios::ostream> output = 0;
    if("-"==outputFile)
    {
        output                  = new ios::ocstream( ios::cstdout );
        aliasing::_(outputFile) = "STDOUT";
    }
    else
    {
        output = new ios::ocstream( outputFile );
    }

    

}
Y_PROGRAM_END()


