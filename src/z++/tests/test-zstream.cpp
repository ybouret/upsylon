#include "y/utest/run.hpp"
#include "y/z++/gzstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/z++/zpipe.hpp"
#include "y/string.hpp"

#include <cstdlib>

using namespace upsylon;

Y_UTEST(iz)
{
	std::cerr << "reading file" << std::endl;
    if( argc > 1)
    {
        const string file_name = argv[1];
		ios::igzstream   fp( file_name );
		ios::ocstream    out( "iz.out", false);

        string line;
        while( fp.gets(line) )
        {
			out << line << '\n';
            line.clear();
        }
    }

}
Y_UTEST_DONE()

Y_UTEST(oz)
{
	std::cerr << "writing foo.gz" << std::endl;
	ios::ogzstream fp( "foo.gz", 7 );
	ios::icstream in( ios::cstdin );

	string line;
	while( in.gets(line) && line != ".quit" )
	{
		//std::cerr << "->" << line << std::endl;
		fp << line << '\n';
		line.clear();
	}

}
Y_UTEST_DONE()

#include "y/z++/zpipe.hpp"

Y_UTEST(def)
{
	size_t level = argc > 1 ? atol( argv[1]  ) : 7;
	size_t chunk = argc > 2 ? atol( argv[2]  ) : 16 * 1024;

	ios::icstream source( ios::cstdin );
	ios::ocstream target( ios::cstdout );

	zlib::zpipe zp( chunk );
	zp.def( target, source, level );



}
Y_UTEST_DONE()

Y_UTEST(inf)
{
	size_t chunk = argc > 1 ? atol( argv[1]  ) : 16 * 1024;

	ios::icstream source( ios::cstdin );
	ios::ocstream target( ios::cstdout );

    zlib::zpipe zp( chunk );
	zp.inf( target, source);



}
Y_UTEST_DONE()


